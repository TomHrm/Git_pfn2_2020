#include <ctype.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include <stdexcept>
#include "multiseq.hpp"

/* This is a function like printf and fprintf which delivers a std::string
   object according to the specified format string <fmt> and the variable
   number of arguments. */

static std::string str_format(const char *fmt, ...)
{
    va_list vl;

    va_start(vl, fmt);
    int size = vsnprintf(0, 0, fmt, vl) + sizeof('\0');
    va_end(vl);

    char *buffer = new char[size];

    va_start(vl, fmt);
    size = vsnprintf(buffer, size, fmt, vl);
    va_end(vl);
    std::string ret = std::string(buffer, size);
    delete[] buffer;
    return ret;
}

static void throw_empty_sequence(const char *filename,
                                 unsigned long num_of_sequences)
{
    std::string msg;
    if (filename != NULL)
    {
        msg = str_format("file \"%s\": sequence number %lu is empty", filename,
                         num_of_sequences);
    } else
    {
        msg =
            str_format("input: sequence number %lu is empty", num_of_sequences);
    }
    throw std::invalid_argument(msg);
}

static void throw_missing_header(const char *filename)
{
    std::string msg;
    if (filename != NULL)
    {
        msg =
            str_format("file \"%s\": missing header for sequence 1", filename);
    } else
    {
        msg = str_format("input: missing header for sequence 1");
    }
    throw std::invalid_argument(msg);
}

/* now the implementation of the methods for class Multiseq follows. Here
   is the header of the fist method, which is the constructor. Note
   that the name of the program is not required as a parameter (as in the
   C-Implementation), because the exception only generates the error message
   without the program-name itself. The latter is combined with the program
   name in the catch-block appearing in the main-function, where the
   program name is available in argv[0]. */
Multiseq::Multiseq(const char *filename, unsigned char *filecontents,
                   unsigned long file_size)
{
    unsigned char *readptr,   /* pointer used for read the sequence */
        *writeptr;            /* pointer used for writing compacted sequence */
    bool firstcol = true,     /* current state is in first colum of any line? */
        insequence = false,   /* current state is in sequene part? */
        firstseq = true;      /* current state is in first sequence */
    unsigned long seqnum = 0; /* current sequence number */

    assert(file_size > 0);

    this->_maximum_sequence_length = 0;
    this->_total_sequence_length = 0;
    for (readptr = writeptr = filecontents; readptr < filecontents + file_size;
         readptr++) /* read the contents of the file char by char
                       white it back to the same memory area, eliminating
                       white spaces including \n from the sequence part.
                       All lines are ending with a \n. In this way
                       Each sequence and each header is a single line.
                       As a side effect we can detect formatting
                       errors in input */
    {
        const unsigned char cc = *readptr;

        if (firstcol)
        {
            if (cc == '>')
            {
                if (firstseq)
                {
                    firstseq = false;
                } else
                {
                    if (!insequence)
                    { /* we are not in the first sequence and have read >. So
                         we must be in the sequence part of the previous
                         sequence. If !insequence, then the previous sequence
                         was empty */
                        throw_empty_sequence(filename, seqnum);
                    }
                    assert(writeptr < filecontents + file_size);
                    /* we are in a sequence and append a \n to the end of the
                       sequence to separate from the possible next sequence */
                    *writeptr++ = '\n';
                }
                /* as have have seen >, we are not in the sequence any more */
                insequence = false;
                /* but are in the header line of the next sequence which gets
                   the next ordinal number */
                seqnum++;
            } else
            {
                /* we have not seed > in the first column, so we are in the
                   sequence part. */
                insequence = true;
            }
        }
        /* we write the complete header back into the memory areqa and we are
           are in a sequence line we write any symbol which ist not a space into
           the memory area we read from. In this way the sequence is compacted,
           as white spaces are elimitanted */
        if (!insequence || !isspace(cc))
        {
            assert(writeptr < filecontents + file_size);
            *writeptr++ = cc;
        }
        /* update whether we are in the first column, as we need it for the
           next iteration */
        firstcol = (cc == '\n') ? true : false;
    }
    if (writeptr >= filecontents + file_size)
    { /* this can only happend if the last sequence was empty and
         so we report an error. */
        throw_empty_sequence(filename, seqnum);
    }
    /* complete the last sequence by appending a \n */
    *writeptr++ = '\n';
    this->sequence_end = writeptr;
    /* we now start again processing the sequence char by char */
    for (readptr = filecontents, firstcol = true; readptr < this->sequence_end;
         readptr++)
    {
        if (firstcol)
        {
            if (*readptr == '>')
            {
                if (this->header_vector.size() > 0)
                {
                    /* we are at the beginning of a header line and have
                       already stored a previous sequence and this
                       does not contain white spaces. We can thus compute its
                       length. by pointer arithmetic involving the pointer
                       readptr referring to the current character and the start
                       of the previous sequence. */
                    const unsigned char *prev_seq =
                        this->sequence_vector.back();
                    const unsigned long seqlen =
                        (unsigned long) ((readptr + 1) - prev_seq - 2);

                    if (seqlen == 0)
                    { /* previous sequence was empty */
                        throw_empty_sequence(filename,
                                             this->header_vector.size());
                    }
                    /* update the two length values */
                    this->_total_sequence_length += seqlen;
                    if (this->_maximum_sequence_length < seqlen)
                    {
                        this->_maximum_sequence_length = seqlen;
                    }
                }
                /* store pointer to header which is the sequence after > */
                this->header_vector.push_back(readptr + 1);
            } else
            {
                /* we are in the first column and not in a header line, i.e.
                   in a sequence line. */
                if (this->header_vector.size() == 0)
                {
                    /* if there was not previous header stored, a header is
                     * missing */
                    throw_missing_header(filename);
                }
                /* as we have eliminated white spaces including newlines,
                   we know that the next line is a complete sequence and we
                   store a pointer to it */
                this->sequence_vector.push_back(readptr);
                /* check that we have number of headers and of sequences stored
                   is the same */
                assert(this->sequence_vector.size() ==
                       this->header_vector.size());
            }
        }
        /* update whether we are in the first column, as we need it for the
           next iteration */
        firstcol = (*readptr == '\n') ? true : false;
    }
    /* perform some checks for the last sequence */
    if (this->header_vector.size() > 0)
    {
        unsigned long seqlen;
        const unsigned char *current_sequence = this->sequence_vector.back();

        assert(this->sequence_end != NULL);
        /* determine length of last sequence */
        seqlen = (unsigned long) (this->sequence_end - current_sequence - 1);
        if (seqlen == 0)
        { /* last sequence is empty */
            throw_empty_sequence(filename, this->header_vector.size());
        }
        /* update the two length values */
        this->_total_sequence_length += seqlen;
        if (this->_maximum_sequence_length < seqlen)
        {
            this->_maximum_sequence_length = seqlen;
        }
    }
}

Multiseq::~Multiseq(void)
{
    // nothing to be done
}

unsigned long Multiseq::num_of_sequences(void) const
{
    assert(this != NULL);
    return this->header_vector.size();
}

unsigned long Multiseq::maximum_sequence_length(void) const
{
    assert(this != NULL);
    return this->_maximum_sequence_length;
}

unsigned long Multiseq::total_sequence_length(void) const
{
    assert(this != NULL);
    return this->_total_sequence_length;
}

unsigned long Multiseq::header_length(unsigned long seqnum) const
{
    assert(this != NULL &&
           this->sequence_vector[seqnum] > this->header_vector[seqnum]);
    return (unsigned long) (this->sequence_vector[seqnum] -
                            this->header_vector[seqnum] - 1);
}

unsigned long Multiseq::sequence_length(unsigned long seqnum) const
{
    assert(this != NULL && seqnum < this->header_vector.size());
    if (seqnum < this->header_vector.size() - 1)
    {
        assert(this->header_vector[seqnum + 1] >
               this->sequence_vector[seqnum] - 1);
        return (unsigned long) (this->header_vector[seqnum + 1] -
                                this->sequence_vector[seqnum] - 2);
    } else
    {
        assert(this->sequence_end > this->sequence_vector[seqnum]);
        return (unsigned long) (this->sequence_end -
                                this->sequence_vector[seqnum] - 1);
    }
}

const unsigned char *Multiseq::sequence_ptr(unsigned long seqnum) const
{
    assert(this != NULL);
    return this->sequence_vector[seqnum];
}

const unsigned char *Multiseq::header_ptr(unsigned long seqnum) const
{
    assert(this != NULL);
    return this->header_vector[seqnum];
}

void Multiseq::show(unsigned long width) const
{
    unsigned long seqnum;
    /* the following code #ifndef ... #endif-code is only compiled if
       the option -DNEBUG is not used. If it is used, then it will
       not be compiled and no assert-statement is executed. */
#ifndef NDEBUG
    unsigned long maximum_sequence_length = this->maximum_sequence_length();
#endif

    for (seqnum = 0; seqnum < this->num_of_sequences(); seqnum++)
    {
        const unsigned char *currentseq;
        unsigned long currentlength;

        fputc('>', stdout);
        fwrite(this->header_ptr(seqnum), sizeof(unsigned char),
               this->header_length(seqnum), stdout);
        fputc('\n', stdout);
        currentseq = this->sequence_ptr(seqnum);
        currentlength = this->sequence_length(seqnum);
        if (width == 0)
        {
            fwrite(currentseq, sizeof *currentseq, currentlength, stdout);
            fputc('\n', stdout);
        } else
        {
            unsigned long offset;
            for (offset = 0; offset < currentlength; offset += width)
            {
                fwrite(currentseq + offset, sizeof *currentseq,
                       (width < currentlength - offset)
                           ? width
                           : (currentlength - offset),
                       stdout);
                fputc('\n', stdout);
            }
        }
        assert(currentlength <= maximum_sequence_length);
    }
}
