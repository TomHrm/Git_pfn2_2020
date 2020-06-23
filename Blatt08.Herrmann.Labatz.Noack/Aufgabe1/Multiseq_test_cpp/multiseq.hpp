#ifndef MULTISEQ_H
#define MULTISEQ_H
#include <vector>

/* implement a class to store multiple sequence stored in a
   file using the simplified multiple fasta format */

class Multiseq
{
  private:
  std::vector<const unsigned char *> header_vector;
  std::vector<const unsigned char *> sequence_vector;
  const unsigned char *sequence_end;
  unsigned long _maximum_sequence_length;
  unsigned long _total_sequence_length;
  public:
  Multiseq(const char *filename,unsigned char *sequence,
           unsigned long totallength);
  ~Multiseq(void);
  unsigned long num_of_sequences(void) const;
  unsigned long maximum_sequence_length(void) const;
  unsigned long total_sequence_length(void) const;
  unsigned long sequence_length(unsigned long seqnum) const;
  const unsigned char *sequence_ptr(unsigned long seqnum) const;
  unsigned long header_length(unsigned long seqnum) const;
  const unsigned char *header_ptr(unsigned long seqnum) const;
  void show(unsigned long) const;
};

#endif
