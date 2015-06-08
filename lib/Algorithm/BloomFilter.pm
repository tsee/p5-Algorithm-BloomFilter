package Algorithm::BloomFilter;
use 5.008001;
use strict;
use warnings;

our $VERSION = '0.01';

require XSLoader;
XSLoader::load('Algorithm::BloomFilter', $VERSION);

1;
__END__

=head1 NAME

Algorithm::BloomFilter - A simple bloom filter data structure

=head1 SYNOPSIS

  use Algorithm::BloomFilter;
  
  my $filter = Algorithm::BloomFilter->new($absolute_nbits, $n_hashes);
  
  $filter->add("foo", "bar", "baz");
  if ($filter->test("bar")) {
    print "Eureka! 'bar' is in!\n";
  }

=head1 DESCRIPTION

This module implements a simple bloom filter in C/XS.

=head1 METHODS

=head2 new

Constructor, takes two arguments: The absolute number of bits to use for the
bloom filter storage (this will be rounded up to the nearest power of 2) and the
number of hash functions to evaluate for each entry.

C<Algorithm::BloomFilter> uses SipHash internally. The C part can also use other
hash functions, but the XS wrapper currently only supports SipHash.

=head2 add

Given a list of values (that will be converted to byte strings),
add those values to the bloom filter.

=head2 test

Given a value (which will be converted to a byte string for this operation),
test whether that value is part of the set represented by the bloom filter.

=head1 CAVEATS

Requires a C<uint64_t> type. Untested on endianness other than x86_64's (little endian).

=head1 SEE ALSO

Wikipedia: L<http://en.wikipedia.org/wiki/Bloom_filter>

=head1 AUTHOR

Steffen Mueller, E<lt>smueller@cpan.orgE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2014-2015 by Steffen Mueller

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.8.0 or,
at your option, any later version of Perl 5 you may have available.

=cut

