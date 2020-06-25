#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "multiseq.h"

/*  A structure to store pointers to memory of base type unsigned char.
    In the following we use the notion vector.
    <space> refers to the memory that stores the pointers.
    <allocated> is the number of entries for which space is allocated.
    <nextfree> is the next entry where a pointer can be stored. */

typedef struct
{
  const unsigned char **space;
  unsigned long nextfree, allocated;
} VectorUcharPtr;

/* append a pointer to the end of the vector referred to by <vec>. */

static void vector_uchar_ptr_push_back(VectorUcharPtr *vec,
                                       const unsigned char *ptr)
{
  assert(vec != NULL);
  if (vec->nextfree >= vec->allocated)
  {
    const unsigned long add = vec->allocated * 0.2 + 128;
    vec->space = realloc(vec->space,
                         (vec->allocated + add) * sizeof *vec->space);
    vec->allocated += add;
  }
  vec->space[vec->nextfree++] = ptr;
}

/* initialize a vector references by <vec>. */

static void vector_uchar_ptr_init(VectorUcharPtr *vec)
{
  assert(vec != NULL);
  vec->space = NULL;
  vec->allocated = vec->nextfree = 0;
}

/* return the last pointer of the vector referenced by <vec> */

static const unsigned char *vector_uchar_ptr_back(const VectorUcharPtr *vec)
{
  assert(vec != NULL && vec->nextfree > 0);
  return vec->space[vec->nextfree - 1];
}

/* return the number of pointers stored in vector references by <vec>. */

static unsigned long vector_uchar_ptr_size(const VectorUcharPtr *vec)
{
  assert(vec != NULL);
  return vec->nextfree;
}

struct Multiseq
{
  VectorUcharPtr header_vector,     /* Vector of header pointers */
                 sequence_vector;   /* Vector of sequence pointers */
  unsigned char *sequence_end;      /* end of memory area storing the
                                       string comprising all sequences
                                       represented by Multiseq */
 unsigned long maximum_sequence_length,  /* maximum length of all sequences */
                total_sequence_length;  /* total length of all sequences */
};

/* generate error message, when empty sequence is detected */

static void error_msg_empty_sequence(const char *program_name,
                                     const char *filename,
                                     unsigned long num_of_sequences)
{
  if (filename != NULL)
  {
    fprintf(stderr,"%s: file \"%s\": sequence number %lu is empty\n",
            program_name,filename,num_of_sequences);
  } else
  {
    fprintf(stderr,"%s: input: sequence number %lu in input is empty\n",
            program_name,num_of_sequences);
  }
}

/* generate error message, when header is missing */

static void error_msg_missing_header(const char *program_name,
                                     const char *filename)
{
  if (filename != NULL)
  {
    fprintf(stderr,"%s: file \"%s\": missing header for sequence 1\n",
            program_name,filename);
  } else
  {
    fprintf(stderr,"%s: input: missing header for sequence 1\n",program_name);
  }
}

/* function creating a Multiseq structure with all initializations */

Multiseq *multiseq_new(const char *program_name,
                       const char *filename, /* input file, use for errors */
                       unsigned char *filecontents, /* refers to entire
                                                       contents of file stored
                                                       in memory */
                       unsigned long file_size) /* length of file */
{
  Multiseq *multiseq = (Multiseq *) malloc(sizeof *multiseq);
  unsigned char *readptr,   /* pointer used for read the sequence */
                *writeptr;  /* pointer used for writing compacted sequence */
  bool firstcol = true,     /* current state is in first colum of any line? */
       insequence = false,  /* current state is in sequene part? */
       firstseq = true;     /* current state is in first sequence */
  unsigned long seqnum = 0; /* current sequence number */

  assert(file_size > 0);
  multiseq->maximum_sequence_length = 0;
  multiseq->total_sequence_length = 0;
  vector_uchar_ptr_init(&multiseq->header_vector);
  vector_uchar_ptr_init(&multiseq->sequence_vector);
  for (readptr = writeptr = filecontents;
       readptr < filecontents + file_size;
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
               we must be in the sequence part of the previous sequence.
               If !insequence, then the previous sequence was empty */
            multiseq_delete(multiseq);
            error_msg_empty_sequence(program_name,filename,seqnum);
            return NULL;
          }
          assert(writeptr < filecontents + file_size);
          /* we are in a sequence and append a \n to the end of the
             sequence to separate from the possible next sequence */
          *writeptr++ = '\n';
        }
        /* as have have seen >, we are not in the sequence any more */
        insequence = false;
        /* but are in the header line of the next sequence which gets the
           next ordinal number */
        seqnum++;
      } else
      {
        /* we have not seed > in the first column, so we are in the sequence
           part. */
        insequence = true;
      }
    }
    /* we write the complete header back into the memory areqa and we are are in a sequence line
       we write any symbol which ist not a space into the memory area
       we read from. In this way the sequence is compacted, as white
       spaces are elimitanted */
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
    multiseq_delete(multiseq);
    error_msg_empty_sequence(program_name,filename,seqnum);
    return NULL;
  }
  /* complete the last sequence by appending a \n */
  *writeptr++ = '\n';
  multiseq->sequence_end = writeptr;
  /* we now start again processing the sequence char by char */
  for (readptr = filecontents, firstcol = true;
       readptr < multiseq->sequence_end;
       readptr++)
  {
    if (firstcol)
    {
      if (*readptr == '>')
      {
        if (vector_uchar_ptr_size(&multiseq->header_vector) > 0)
        {
          /* we are at the beginning of a header line and have
             already stored a previous sequence and this
             does not contain white spaces. We can thus compute its length.
             by pointer arithmetic involving the pointer readptr
             referring to the current character and the start of the
             previous sequence. */
          const unsigned char *prev_seq
            = vector_uchar_ptr_back(&multiseq->sequence_vector);
          const unsigned long seqlen
            = (unsigned long) ((readptr + 1) - prev_seq - 2);

          if (seqlen == 0)
          { /* previous sequence was empty */
            multiseq_delete(multiseq);
            error_msg_empty_sequence(program_name,filename,
                                     vector_uchar_ptr_size(
                                       &multiseq->header_vector));
            return NULL;
          }
          /* update the two length values */
          multiseq->total_sequence_length += seqlen;
          if (multiseq->maximum_sequence_length < seqlen)
          {
            multiseq->maximum_sequence_length = seqlen;
          }
        }
        /* store pointer to header which is the sequence after > */
        vector_uchar_ptr_push_back(&multiseq->header_vector,readptr + 1);
      } else
      {
        /* we are in the first column and not in a header line, i.e.
           in a sequence line. */
        if (vector_uchar_ptr_size(&multiseq->header_vector) == 0)
        {
          /* if there was not previous header stored, a header is missing */
          multiseq_delete(multiseq);
          error_msg_missing_header(program_name,filename);
          return NULL;
        }
        /* as we have eliminated white spaces including newlines,
           we know that the next line is a complete sequence and we
           store a pointer to it */
        vector_uchar_ptr_push_back(&multiseq->sequence_vector,readptr);
        /* check that we have number of headers and of sequences stored
           is the same */
        assert(multiseq->sequence_vector.nextfree ==
               multiseq->header_vector.nextfree);
      }
    }
    /* update whether we are in the first column, as we need it for the
       next iteration */
    firstcol = (*readptr == '\n') ? true : false;
  }
  /* perform some checks for the last sequence */
  if (vector_uchar_ptr_size(&multiseq->header_vector) > 0)
  {
    unsigned long seqlen;
    const unsigned char *current_sequence
      = vector_uchar_ptr_back(&multiseq->sequence_vector);

    assert(multiseq->sequence_end != NULL);
    /* determine length of last sequence */
    seqlen = (unsigned long) (multiseq->sequence_end - current_sequence - 1);
    if (seqlen == 0)
    { /* last sequence is empty */
      multiseq_delete(multiseq);
      error_msg_empty_sequence(program_name,filename,
                               vector_uchar_ptr_size(&multiseq->header_vector));
      return NULL;
    }
    /* update the two length values */
    multiseq->total_sequence_length += seqlen;
    if (multiseq->maximum_sequence_length < seqlen)
    {
      multiseq->maximum_sequence_length = seqlen;
    }
  }
  return multiseq;
}

/* delete a multiseq including the vectors storing the pointers
   to the headers and the sequences. While the Multiseq refers
   to memory storing the headers and sequences, it is not responsible
   for freeing this memory area. */

void multiseq_delete(Multiseq *multiseq)
{
  if (multiseq != NULL)
  {
    free(multiseq->header_vector.space);
    free(multiseq->sequence_vector.space);
    free(multiseq);
  }
}

/* return number of sequences */

unsigned long multiseq_num_of_sequences(const Multiseq *multiseq)
{
  assert(multiseq != NULL);
  return vector_uchar_ptr_size(&multiseq->header_vector);
}

/* return maximum sequence length */

unsigned long multiseq_maximum_sequence_length(const Multiseq *multiseq)
{
  assert(multiseq != NULL);
  return multiseq->maximum_sequence_length;
}

/* return total sequence length */

unsigned long multiseq_total_sequence_length(const Multiseq *multiseq)
{
  assert(multiseq != NULL);
  return multiseq->total_sequence_length;
}

/* return length of header of sequence with given sequence number */

unsigned long multiseq_header_length(const Multiseq *multiseq,
                                     unsigned long seqnum)
{
  assert(multiseq != NULL &&
         multiseq->sequence_vector.space[seqnum] >
         multiseq->header_vector.space[seqnum]);
  return (unsigned long) (multiseq->sequence_vector.space[seqnum] -
                          multiseq->header_vector.space[seqnum] - 1);
}

/* return length of sequence with given sequence number */

unsigned long multiseq_sequence_length(const Multiseq *multiseq,
                                       unsigned long seqnum)
{
  assert(multiseq != NULL &&
         seqnum < vector_uchar_ptr_size(&multiseq->header_vector));
  if (seqnum < vector_uchar_ptr_size(&multiseq->header_vector) - 1)
  {
    assert(multiseq->header_vector.space[seqnum+1] >
           multiseq->sequence_vector.space[seqnum] - 1);
    return (unsigned long) (multiseq->header_vector.space[seqnum+1] -
                            multiseq->sequence_vector.space[seqnum] - 2);
  } else
  {
    assert(multiseq->sequence_end >
           multiseq->sequence_vector.space[seqnum]);
    return (unsigned long) (multiseq->sequence_end -
                            multiseq->sequence_vector.space[seqnum] - 1);
  }
}

/* return pointer to sequence with given sequence number */

const unsigned char *multiseq_sequence_ptr(const Multiseq *multiseq,
                                           unsigned long seqnum)
{
  assert(multiseq != NULL);
  return multiseq->sequence_vector.space[seqnum];
}

/* return pointer to header with given sequence number */

const unsigned char *multiseq_header_ptr(const Multiseq *multiseq,
                                         unsigned long seqnum)
{
  assert(multiseq != NULL);
  return multiseq->header_vector.space[seqnum];
}

/* output the headers and sequences formatted to the given width. */

void multiseq_show(unsigned long width,const Multiseq *multiseq)
{
  unsigned long seqnum;
  /* the following code #ifndef ... #endif-code is only compiled if
     the option -DNEBUG is not used. If it is used, then it will
     not be compiled and no assert-statement is executed. */
#ifndef NDEBUG
  unsigned long maximum_sequence_length
    = multiseq_maximum_sequence_length(multiseq);
#endif

  for (seqnum = 0; seqnum < multiseq_num_of_sequences(multiseq); seqnum++)
  {
    const unsigned char *currentseq;
    unsigned long currentlength;

    fputc('>',stdout);
    fwrite(multiseq_header_ptr(multiseq, seqnum), sizeof (unsigned char),
           multiseq_header_length(multiseq, seqnum), stdout);
    fputc('\n',stdout);
    currentseq = multiseq_sequence_ptr(multiseq, seqnum);
    currentlength = multiseq_sequence_length(multiseq, seqnum);
    if (width == 0)
    {
      fwrite(currentseq, sizeof *currentseq,currentlength,stdout);
      fputc('\n',stdout);
    } else
    {
      unsigned long offset;
      for (offset = 0; offset < currentlength; offset += width)
      {
        fwrite(currentseq + offset, sizeof *currentseq,
               (width < currentlength - offset) ? width
                                                : (currentlength - offset),
               stdout);
        fputc('\n',stdout);
      }
    }
    assert(currentlength <= maximum_sequence_length);
  }
}

/* no need to implement this for the Multiseq in C++ exercise */

const unsigned char *multiseq_ID_ptr(const Multiseq *multiseq,
                                     unsigned long *id_len,
                                     unsigned long seqnum)
{
  const unsigned char *header_ptr
    = multiseq_header_ptr(multiseq,seqnum);
  unsigned long header_len = multiseq_header_length(multiseq,seqnum);
  const unsigned char *first_pipe_sym, *second_pipe_sym = NULL;
  const char id_separator = '|';

  assert(header_ptr != NULL);
  first_pipe_sym
    = (const unsigned char *) memchr(header_ptr,id_separator,header_len);
  if (first_pipe_sym != NULL)
  {
    second_pipe_sym = (const unsigned char *)
         memchr(first_pipe_sym+1,id_separator,
                (size_t) (header_ptr + header_len - (first_pipe_sym+1)));
    if (second_pipe_sym != NULL)
    {
      *id_len = (unsigned long) (second_pipe_sym - (first_pipe_sym + 1));
      return first_pipe_sym + 1;
    }
  }
  return NULL;
}
