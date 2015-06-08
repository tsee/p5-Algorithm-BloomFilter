package Algorithm::BloomFilter;
use 5.008001;
use strict;
use warnings;

require Exporter;

our $VERSION = '0.01';

require XSLoader;
XSLoader::load('Algorithm::BloomFilter', $VERSION);


1;
__END__

=head1 NAME

Algorithm::BloomFilter - ...

=head1 SYNOPSIS

  use Algorithm::BloomFilter;

=head1 DESCRIPTION

=head1 AUTHOR

Steffen Mueller, E<lt>smueller@cpan.orgE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2014 by Steffen Mueller

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.8.0 or,
at your option, any later version of Perl 5 you may have available.

=cut

