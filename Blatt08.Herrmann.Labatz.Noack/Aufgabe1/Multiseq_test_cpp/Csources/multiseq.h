#ifndef MULTISEQ_H
#define MULTISEQ_H

/* <Multiseq> is a class to parse multiple Fasta-files and provide
   access to sequences, their headers and sequence lengths for given
   sequences numbers. */
typedef struct Multiseq Multiseq;

/* Return a new <Multiseq> Object from the given filecontents of
   length <file_size>. The name of the program in which the
   function is called is provided via the the first argument.
   The original filename is given as second argument.
   Use NULL, if this is not available. */

Multiseq *multiseq_new(const char *program_name,const char *filename,
                       unsigned char *filecontents,unsigned long file_size);

/* Free the memory of <multiseq> */
void multiseq_delete(Multiseq *multiseq);

/* Return the number of sequences in <multiseq> */
unsigned long multiseq_num_of_sequences(const Multiseq *multiseq);

/* Return the sum of the length of all sequences in the <multiseq>. */
unsigned long multiseq_total_sequence_length(const Multiseq *multiseq);

/* Return the maximum length of all sequences in <multiseq> */
unsigned long multiseq_maximum_sequence_length(const Multiseq *multiseq);

/* Return the length of sequence with sequence number <seqnum> in
   <multiseq>. Sequence numbers are counted from 0. */
unsigned long multiseq_sequence_length(const Multiseq *multiseq,
                                       unsigned long seqnum);
/* Return a pointer to the sequence <seqnum> of <multiseq>, do not attempt to
   free it. */
const unsigned char *multiseq_sequence_ptr(const Multiseq *multiseq,
                                           unsigned long seqnum);
/* Return the length of sequence with number <seqnum> in <multiseq>. */
unsigned long multiseq_header_length(const Multiseq *multiseq,
                                     unsigned long seqnum);
/* Return a pointer to the header of sequence <seqnum> in <multiseq>, do not
   attempt to free it. The header is not necessarily 0-terminated. */
const unsigned char *multiseq_header_ptr(const Multiseq *multiseq,
                                         unsigned long seqnum);

/* Print the content of <multiseq> to stdout with the sequence in
   line of width <width>. */
void multiseq_show(unsigned long width,const Multiseq *multiseq);

/* Return a pointer to the ID in the sequence header of sequence number
   seqnum. The ID is the leftmost string in the header delimited by the
   symbol | on both sides. The string length of the ID is stored in the
   memory_location referred to by id_len.
   If no ID occurs, the function returns NULL and the content of the
   memory location referred to by id_len does not change. */
const unsigned char *multiseq_ID_ptr(const Multiseq *multiseq,
                                     unsigned long *id_len,
                                     unsigned long seqnum);

#endif
