#define PERL_NO_GET_CONTEXT     /* we want efficiency */
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include "bloom.h"

MODULE = Algorithm::BloomFilter		PACKAGE = Algorithm::BloomFilter
PROTOTYPES: DISABLE

TYPEMAP: <<HERE
bloom_t*	O_OBJECT

OUTPUT

O_OBJECT
  sv_setref_pv( $arg, CLASS, (void*)$var );

INPUT

O_OBJECT
  if ( sv_isobject($arg) && (SvTYPE(SvRV($arg)) == SVt_PVMG) )
    $var = ($type)SvIV((SV*)SvRV( $arg ));
  else
    croak( \"${Package}::$func_name() -- $var is not a blessed SV reference\" );

HERE

bloom_t *
new(const char *CLASS, UV n_bits, UV k_hashes)
  CODE:
    RETVAL = bl_alloc(n_bits, k_hashes, bl_siphash);
  OUTPUT: RETVAL

void
DESTROY(bloom_t *bl)
  CODE:
    bl_free(bl);

void
add(bloom_t *bl, SV *value)
  PREINIT:
    char *str;
    STRLEN len;
  CODE:
    str = SvPVbyte(value, len);
    bl_add(bl, str, len);

IV
test(bloom_t *bl, SV *value)
  PREINIT:
    char *str;
    STRLEN len;
  CODE:
    str = SvPVbyte(value, len);
    RETVAL = (IV)bl_test(bl, str, len);
  OUTPUT: RETVAL

