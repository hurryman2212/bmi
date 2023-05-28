#define _GNU_SOURCE

#include "bmi.h"

#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>

fd_set fds;

bitset_t bitset[FD_SETSIZE / (sizeof(bitset_t) * 8)] = {0};

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "usage: %s <NUM_BITIDX> [BITIDX ...]\n", argv[0]);
    fprintf(stderr, "(`argc > 3` must be `true`)\n");
    exit(EXIT_FAILURE);
  }
  int num_bitidx = atoi(argv[1]);

  FD_ZERO(&fds);

  printf("-SET TEST START------------------------------------------\n");

  /* Set test */

  for (int i = 2; i < (num_bitidx + 2); ++i) {
    int fd = atoi(argv[i]);
    printf("FD: %d\n", fd);
    set_bit_nonatomic(bitset, fd);
    FD_SET(fd, &fds);
  }

  printf("bitset <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (bitset[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("fds <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (((bitset_t *)&fds)[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("-SET TEST COMPLETE------------------------------------------\n");

  printf("-UNSET TEST START------------------------------------------\n");

  /* Unset test */

  for (int i = 2; i < (num_bitidx + 2); ++i) {
    int fd = atoi(argv[i]);
    unset_bit_nonatomic(bitset, fd);
    FD_CLR(fd, &fds);
  }

  printf("bitset <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (bitset[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("fds <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (((bitset_t *)&fds)[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("-UNSET TEST COMPLETE------------------------------------------\n");

  printf("-SET AGAIN START------------------------------------------\n");

  /* Set again before next test */

  for (int i = 2; i < (num_bitidx + 2); ++i) {
    int fd = atoi(argv[i]);
    set_bit_nonatomic(bitset, fd);
    FD_SET(fd, &fds);
  }

  printf("bitset <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (bitset[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("fds <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (((bitset_t *)&fds)[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("-SET AGAIN COMPLETE------------------------------------------\n");

  printf("-TESTING BIT START------------------------------------------\n");

  /* Testing of bit test */

  for (int i = 0; i < FD_SETSIZE; ++i) {
    if (test_bit(bitset, i))
      printf("[BITSET] Testing result for #%d bit: 1\n", i);
  }

  for (int i = 0; i < FD_SETSIZE; ++i) {
    if (FD_ISSET(i, &fds))
      printf("[FDS] Testing result for #%d bit: 1\n", i);
  }

  printf("bitset <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (bitset[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("fds <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (((bitset_t *)&fds)[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("-TESTING BIT COMPLETE------------------------------------------\n");

  printf("-SEARCH BIT TEST START------------------------------------------\n");

  /* Search lowest test */

  int first_fd = 0;
  int last_fd = atoi(argv[num_bitidx + 1]);
  while (1) {
    printf("[BITSET]\n");
    int fd_num = search_lowest_bit(bitset, first_fd, last_fd);
    if (fd_num < 0) {
      printf("fd_num < 0; Exiting loop!\n");
      break;
    }
    first_fd = fd_num + 1;
    printf("Found fd number %d in u64_fd_set[]!\n", fd_num);

    printf("test_bit_nonatomic(bitset, fd_num) returned %d! (expect 1)\n",
           test_bit(bitset, fd_num));
  }

  first_fd = 0;
  last_fd = atoi(argv[num_bitidx + 1]);
  while (1) {
    printf("[FDS]\n");
    int fd_num = search_lowest_bit((bitset_t *)&fds, first_fd, last_fd);
    if (fd_num < 0) {
      printf("fd_num < 0; Exiting loop!\n");
      break;
    }
    first_fd = fd_num + 1;
    printf("Found fd number %d in u64_fd_set[]!\n", fd_num);

    printf("test_bit_nonatomic(bitset, fd_num) returned %d! (expect 1)\n",
           test_bit(bitset, fd_num));
  }

  printf("bitset <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (bitset[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("fds <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (((bitset_t *)&fds)[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("-SEARCH BIT TEST END------------------------------------------\n");

  printf("-CONSUME BIT TEST START------------------------------------------\n");

  /* Consume lowest test */

  first_fd = 0;
  last_fd = atoi(argv[num_bitidx + 1]);
  while (1) {
    printf("[BITSET]\n");
    int fd_num = consume_lowest_bit(bitset, first_fd, last_fd);
    if (fd_num < 0) {
      printf("fd_num < 0; Exiting loop!\n");
      break;
    }
    first_fd = fd_num + 1;
    printf("Found fd number %d in u64_fd_set[] and consumed!\n", fd_num);

    printf("test_bit_nonatomic(bitset, fd_num) returned %d! (expect 0)\n",
           test_bit(bitset, fd_num));
  }

  first_fd = 0;
  last_fd = atoi(argv[num_bitidx + 1]);
  while (1) {
    printf("[FDS]\n");
    int fd_num = consume_lowest_bit((bitset_t *)&fds, first_fd, last_fd);
    if (fd_num < 0) {
      printf("fd_num < 0; Exiting loop!\n");
      break;
    }
    first_fd = fd_num + 1;
    printf("Found fd number %d in u64_fd_set[] and consumed!\n", fd_num);

    printf("test_bit_nonatomic(bitset, fd_num) returned %d! (expect 0)\n",
           test_bit(bitset, fd_num));
  }

  printf("bitset <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (bitset[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("fds <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (((bitset_t *)&fds)[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("-CONSUME BIT TEST END------------------------------------------\n");

  printf("-SET AGAIN START------------------------------------------\n");

  /* Set again before next test */

  for (int i = 2; i < (num_bitidx + 2); ++i) {
    int fd = atoi(argv[i]);
    set_bit_nonatomic(bitset, fd);
    FD_SET(fd, &fds);
  }

  printf("bitset <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (bitset[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("fds <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (((bitset_t *)&fds)[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("-SET AGAIN COMPLETE------------------------------------------\n");

  printf("-COMPARE TEST START------------------------------------------\n");

  /* Compare bitset test */

  first_fd = 0;
  last_fd = atoi(argv[num_bitidx + 1]);
  while (1) {
    int fd_num =
        search_lowest_common_bit(bitset, (bitset_t *)&fds, first_fd, last_fd);
    if (fd_num < 0) {
      printf("fd_num < 0; Exiting loop!\n");
      break;
    }
    first_fd = fd_num + 1;
    printf("Found common fd number %d!\n", fd_num);
  }

  printf("bitset <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (bitset[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("fds <= [");
  for (unsigned long i = 0; i < (__FD_SETSIZE / (sizeof(bitset_t) * 8)); ++i) {
    printf(" ");
    for (unsigned long j = 0; j < (sizeof(bitset_t) * 8); ++j) {
      if (((uint64_t *)&fds)[i] & (1UL << j))
        printf("1(%lu)", (i * 64) + j);
      else
        printf("0");
    }
  }
  printf(" ]\n");

  printf("-COMPARE TEST COMPLETE------------------------------------------\n");

  return 0;
}
