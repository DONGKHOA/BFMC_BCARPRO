/*!
 * @file qffmath.c
 * @author J. Camilo Gomez C.
 * @note This file is part of the qLibs distribution.
 **/

#include "qffmath.h"

#ifndef QLIBS_USE_STD_MATH
#include <stdint.h>
#include <string.h>
#include <float.h>

/*cppcheck-suppress misra-c2012-20.7 */
#define cast_reinterpret( dst, src, dst_type )                              \
(void)memcpy( &dst, &src, sizeof(dst_type) )                                \

static float qFFMath_CalcCbrt( float x , bool r );
/*============================================================================*/
float _qFFMath_GetAbnormal( const int i )
{
    static const uint32_t u_ab[ 2 ] = { 0x7F800000U, 0x7FBFFFFFU };
    static float f_ab[ 2 ] = { 0.0F, 0.0F };
    static bool init = true;

    if ( init ) {
        /*cppcheck-suppress misra-c2012-21.15 */
        (void)memcpy( f_ab, u_ab, sizeof(f_ab) );
        init = false;
    }

    return f_ab[ i ];
}
/*============================================================================*/
int qFFMath_FPClassify( const float f )
{
    uint32_t u = 0U;
    int retVal;

    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( u, f, uint32_t );
    u &= 0x7FFFFFFFU;

    if ( 0U == u ) {
        retVal = QFFM_FP_ZERO;
    }
    else if ( u < 0x00800000U ) {
        retVal = QFFM_FP_SUBNORMAL;
    }
    else if ( u < 0x7F800000U ) {
        retVal = QFFM_FP_NORMAL;
    }
    else if ( 0x7F800000U == u ) {
        retVal = QFFM_FP_INFINITE;
    }
    else {
        retVal = QFFM_FP_NAN;
    }

    return retVal;
}
/*============================================================================*/
bool qFFMath_IsNaN( const float x )
{
    return ( QFFM_FP_NAN == qFFMath_FPClassify( x ) );
}
/*============================================================================*/
bool qFFMath_IsInf( const float x )
{
    return ( QFFM_FP_INFINITE == qFFMath_FPClassify( x ) );
}
/*============================================================================*/
bool qFFMath_IsFinite( const float x )
{
    return ( qFFMath_FPClassify( x ) < QFFM_FP_INFINITE );
}
/*============================================================================*/
bool qFFMath_IsNormal( const float x )
{
    return ( qFFMath_FPClassify( x ) == QFFM_FP_NORMAL );
}
/*============================================================================*/
bool qFFMath_isAlmostEqual( const float a,
                            const float b,
                            const float tol )
{
    return ( qFFMath_Abs( a - b ) <= qFFMath_Abs( tol ) );
}
/*============================================================================*/
bool qFFMath_isEqual( const float a,
                      const float b )
{
    return ( qFFMath_Abs( a - b ) <= FLT_MIN );
}
/*============================================================================*/
float qFFMath_Abs( float x )
{
    return ( x < 0.0F ) ? -x : x;
}
/*============================================================================*/
float qFFMath_Recip( float x )
{
    uint32_t y = 0U;
    float z = 0.0F;

    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( y, x, uint32_t );
    y = 0x7EF311C7U - y;
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( z, y, float );

    return z*( 2.0F - ( x*z ) );
}
/*============================================================================*/
float qFFMath_Sqrt( float x )
{
    float retVal;

    if ( x < 0.0F ) {
        retVal = QFFM_NAN;
    }
    else if ( QFFM_FP_ZERO == qFFMath_FPClassify( x ) ) {
        retVal = 0.0F;
    }
    else {
        uint32_t y = 0U;
        float z = 0.0F;
        /*cppcheck-suppress misra-c2012-21.15 */
        cast_reinterpret( y, x, uint32_t );
        y = ( ( y - 0x00800000U ) >> 1U ) + 0x20000000U;
        /*cppcheck-suppress misra-c2012-21.15 */
        cast_reinterpret( z, y, float );
        z = ( ( x/z ) + z ) * 0.5F;
        retVal = 0.5F*( ( x/z ) + z );
    }

    return retVal;
}
/*============================================================================*/
float qFFMath_RSqrt( float x )
{
    float retVal;

    if ( x < 0.0F ) {
        retVal = QFFM_NAN;
    }
    else if ( QFFM_FP_ZERO == qFFMath_FPClassify( x ) ) {
        retVal = QFFM_INFINITY;
    }
    else {
        uint32_t y = 0U;
        float z = 0.5F*x;
        /*cppcheck-suppress misra-c2012-21.15 */
        cast_reinterpret( y, x, uint32_t );
        y = 0x5F375A86U - ( y >> 1U );
        /*cppcheck-suppress misra-c2012-21.15 */
        cast_reinterpret( x, y, float );
        retVal = x*( 1.5F - ( z*x*x ) );
    }

    return retVal;
}
/*============================================================================*/
static float qFFMath_CalcCbrt( float x , bool r )
{
    float retVal, y = 0.0F, c, d;
    const float k[ 3 ] = { 1.752319676F, 1.2509524245F, 0.5093818292F };
    uint32_t i = 0U;
    bool neg = false;

    if ( x < 0.0F ) {
        x = -x;
        neg = true;
    }
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( i, x, uint32_t );
    i = 0x548C2B4BU - ( i/3U );
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( y, i, float );
    c = x*y*y*y;
    y = y*( k[ 0 ] - ( c*( k[ 1 ] - ( k[ 2 ]*c ) ) ) );

    d = x*y*y;
    c = 1.0F - ( d*y );
    retVal = 1.0F + ( 0.333333333333F*c );
    retVal *= ( r ) ? y : d;
    return ( neg )? -retVal : retVal;
}
/*============================================================================*/
float qFFMath_Cbrt( float x )
{
    return qFFMath_CalcCbrt( x, false );
}
/*============================================================================*/
float qFFMath_RCbrt( float x )
{
    return ( QFFM_FP_ZERO == qFFMath_FPClassify( x ) ) ? QFFM_INFINITY
                                                       : qFFMath_CalcCbrt( x, true );
}
/*============================================================================*/
float qFFMath_Round( float x )
{
    x += 12582912.0F;
    x -= 12582912.0F;
    return x;
}
/*============================================================================*/
float qFFMath_Floor( float x )
{
    return qFFMath_Round( x - 0.5F );
}
/*============================================================================*/
float qFFMath_Ceil( float x )
{
    return qFFMath_Round( x + 0.5F );
}
/*============================================================================*/
float qFFMath_Trunc( float x )
{
    /*cstat -CERT-FLP36-C -CERT-FLP34-C*/
    return (float)( (int32_t)x );
    /*cstat +CERT-FLP36-C +CERT-FLP34-C*/
}
/*============================================================================*/
float qFFMath_Frac( float x )
{
    return x - qFFMath_Trunc( x );
}
/*============================================================================*/
float qFFMath_Remainder( float x,
                         float y )
{
     return x - ( y*qFFMath_Floor( x/y ) );
}
/*============================================================================*/
float qFFMath_Mod( float x,
                   float y )
{
    return ( QFFM_FP_ZERO == qFFMath_FPClassify( x )) ? QFFM_NAN
                                                      : ( x - ( y*qFFMath_Trunc( x/y ) ) );
}
/*============================================================================*/
float qFFMath_Sin( float x )
{
    float y;

    if ( qFFMath_Abs( x ) <= 0.0066F ) {
        y = x;
    }
    else {
        x *= -QFFM_1_PI;
        y = x + 25165824.0F;
        x -= y - 25165824.0F;
        x *= qFFMath_Abs( x ) - 1.0F;
        y = x*( ( 3.5841304553896F*qFFMath_Abs( x ) ) + 3.1039673861526F );
    }
    return y;
}
/*============================================================================*/
float qFFMath_Cos( float x )
{
    float y;
    const float abs_x = qFFMath_Abs( x );

    if ( qFFMath_isEqual( abs_x, QFFM_PI_2 ) ) {
        y = 1.0e-12F;
    }
    else {
        y = qFFMath_Sin( x + QFFM_PI_2 );
    }

    return y;
}
/*============================================================================*/
float qFFMath_Tan( float x )
{
    return qFFMath_Sin( x )/qFFMath_Cos( x );
}
/*============================================================================*/
float qFFMath_ASin( float x )
{
    x = qFFMath_Sqrt( 1.0F + x ) - qFFMath_Sqrt( 1.0F - x );
    return x*( ( 0.131754508171F*qFFMath_Abs( x ) ) + 0.924391722181F );
}
/*============================================================================*/
float qFFMath_ACos( float x )
{
    return QFFM_PI_2 - qFFMath_ASin( x );
}
/*============================================================================*/
float qFFMath_ATan( float x )
{
    float abs_x;

    x /= qFFMath_Abs( x ) + 1.0F;
    abs_x = qFFMath_Abs( x );

    return x*( ( abs_x*( ( -1.45667498914F*abs_x ) + 2.18501248371F ) ) + 0.842458832225F );
}
/*============================================================================*/
float qFFMath_ATan2( float y, float x )
{
    float t, f;

    t = QFFM_PI - ( ( y < 0.0F ) ? 6.283185307F : 0.0F );
    f = ( qFFMath_Abs( x ) <= FLT_MIN ) ? 1.0F : 0.0F;
    y = qFFMath_ATan( y/( x + f ) ) + ( ( x < 0.0F ) ? t : 0.0F );

    return y + ( f*( ( 0.5F*t ) - y ) );
}
/*============================================================================*/
float qFFMath_Exp2( float x )
{
    float retVal;

    if ( x <= -126.0F ) {
        retVal = 0.0F;
    }
    else if ( x > 128.0F ) {
        retVal = QFFM_INFINITY;
    }
    else {
        float ip, fp;
        float ep_f = 0.0F;
        int32_t ep_i;

        ip = qFFMath_Floor( x + 0.5F );
        fp = x - ip;
        /*cstat -MISRAC2012-Rule-10.1_R6 -CERT-FLP34-C*/
        ep_i = ( (int32_t)( ip ) + 127 ) << 23;
        /*cstat +MISRAC2012-Rule-10.1_R6 +CERT-FLP34-C*/
        x = 1.535336188319500e-4F;
        x = ( x*fp ) + 1.339887440266574e-3F;
        x = ( x*fp ) + 9.618437357674640e-3F;
        x = ( x*fp ) + 5.550332471162809e-2F;
        x = ( x*fp ) + 2.402264791363012e-1F;
        x = ( x*fp ) + 6.931472028550421e-1F;
        x = ( x*fp ) + 1.0F;
        cast_reinterpret( ep_f, ep_i, float );
        retVal = ep_f*x;
    }

    return retVal;
}
/*============================================================================*/
float qFFMath_Log2( float x )
{
    float retVal;

    if ( x < 0.0F ) {
        retVal = QFFM_NAN;
    }
    else if ( QFFM_FP_ZERO == qFFMath_FPClassify( x ) ) {
        retVal = -QFFM_INFINITY;
    }
    else {
        float z, px;
        int32_t ip, fp;
        int32_t val_i = 0;

        cast_reinterpret( val_i, x, int32_t );
        /*cstat -MISRAC2012-Rule-10.1_R6*/
        fp = val_i & 8388607;
        ip = val_i & 2139095040;
        fp |= 1065353216;
        cast_reinterpret( x, fp, float );
        ip >>= 23;
        ip -= 127;
        /*cstat +MISRAC2012-Rule-10.1_R6*/
        if ( x > QFFM_SQRT2 ) {
            x *= 0.5F;
            ++ip;
        }
        x -= 1.0F;
        px = 7.0376836292e-2F;
        px = ( px*x ) - 1.1514610310e-1F;
        px = ( px*x ) + 1.1676998740e-1F;
        px = ( px*x ) - 1.2420140846e-1F;
        px = ( px*x ) + 1.4249322787e-1F;
        px = ( px*x ) - 1.6668057665e-1F;
        px = ( px*x ) + 2.0000714765e-1F;
        px = ( px*x ) - 2.4999993993e-1F;
        px = ( px*x ) + 3.3333331174e-1F;
        z = x*x;
        z = ( x*z*px ) - ( 0.5F*z ) + x;
        z *= QFFM_LOG2E;
        /*cstat -CERT-FLP36-C*/
        retVal = ( (float)ip ) + z;
        /*cstat +CERT-FLP36-C*/
    }

    return retVal;
}
/*============================================================================*/
float qFFMath_Exp( float x )
{
    return qFFMath_Exp2( QFFM_LOG2E*x );
}
/*============================================================================*/
float qFFMath_Exp10( float x )
{
    return qFFMath_Exp2( 3.32192809F*x );
}
/*============================================================================*/
float qFFMath_Log( float x )
{
    return QFFM_LN2*qFFMath_Log2(x);
}
/*============================================================================*/
float qFFMath_Log10( float x )
{
    return 0.301029996F*qFFMath_Log2(x);
}
/*============================================================================*/
float qFFMath_Pow( float b,
                   float e )
{
    return qFFMath_Exp2( e*qFFMath_Log2( b ) );
}
/*============================================================================*/
float qFFMath_Sinh( float x )
{
    const float epx = qFFMath_Exp( x );
    const float enx = 1.0F/epx;
    return 0.5F*( epx - enx );
}
/*============================================================================*/
float qFFMath_Cosh( float x )
{
    const float epx = qFFMath_Exp( x );
    const float enx = 1.0F/epx;
    return 0.5F*( epx + enx );
}
/*============================================================================*/
float qFFMath_Tanh( float x )
{
    x = qFFMath_Exp( -2.0F*x );
    return ( 1.0F - x )/( 1.0F + x );
}
/*============================================================================*/
float qFFMath_ASinh( float x )
{
    return qFFMath_Log( x + qFFMath_Sqrt( ( x*x ) + 1.0F ) );
}
/*============================================================================*/
float qFFMath_ACosh( float x )
{
    return ( x < 1.0F ) ? QFFM_NAN
                        : qFFMath_Log( x + qFFMath_Sqrt( ( x*x ) - 1.0F ) );
}
/*============================================================================*/
float qFFMath_ATanh( float x )
{
    return qFFMath_Log( ( 1.0F + x )/( 1.0F - x ) )*0.5F;
}
/*============================================================================*/
float qFFMath_Erf( float x )
{
    float retVal;

    if ( x >= 6.912F ) {
        retVal = 1.0F;
    }
    else {
        x = qFFMath_Exp( 3.472034176F*x );
        retVal = ( x/( ( qFFMath_Abs( x ) + 1.0F )*2.0F ) ) - 1.0F;
    }

    return retVal;
}
/*============================================================================*/
float qFFMath_Erfc( float x )
{
    return 1.0F - qFFMath_Erf( x );
}
/*============================================================================*/
float qFFMath_Max( float x,
                   float y )
{
    return ( x > y ) ? x : y;
}
/*============================================================================*/
float qFFMath_Min( float x,
                   float y )
{
    return ( x < y ) ? x : y;
}
/*============================================================================*/
float qFFMath_RExp( float x,
                    int32_t *pw2 )
{
    uint32_t lu = 0U, iu;
    int32_t i = 0;
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( lu, x, uint32_t );
    iu  = ( lu >> 23U ) & 0x000000FFU;  /* Find the exponent (power of 2) */
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( i, iu, int32_t );
    i -= 0x7E;
    pw2[ 0 ] = (int)i;
    lu &= 0x807FFFFFU; /* strip all exponent bits */
    lu |= 0x3F000000U; /* mantissa between 0.5 and 1 */
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( x, lu, float );

    return x;
}
/*============================================================================*/
float qFFMath_LDExp( float x,
                     int32_t pw2 )
{
    uint32_t lu = 0U, eu;
    int32_t e = 0;
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( lu, x, uint32_t );
    eu = ( lu >> 23U ) & 0x000000FFU;
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( e, eu, int32_t );
    e += pw2;
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( eu, e, uint32_t );
    lu = ( ( eu & 0xFFU ) << 23U ) | ( lu & 0x807FFFFFU );
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( x, lu, float );

    return x;
}
/*============================================================================*/
float qFFMath_Hypot( float x,
                     float y )
{
    float retVal;
    /*cstat -MISRAC2012-Rule-13.5*/
    if ( qFFMath_IsFinite( x ) && qFFMath_IsFinite( y ) ) {
        float a, b, an, bn;;
        int32_t e = 0;

        if ( x >= y ) {
            a = x;
            b = y;
        }
        else {
            a = y;
            b = x;
        }
        /* Write a = an * 2^e, b = bn * 2^e with 0 <= bn <= an < 1.*/
        an = qFFMath_RExp( a, &e );
        bn = qFFMath_LDExp( b, -e );
        retVal = qFFMath_Sqrt( ( an*an ) + ( bn*bn ) );
        retVal = qFFMath_LDExp( retVal, e );
    }
    else {
        retVal = ( qFFMath_IsInf( x ) || qFFMath_IsInf( y ) ) ? QFFM_INFINITY
                                                              : QFFM_NAN;
    }
    /*cstat +MISRAC2012-Rule-13.5*/
    return retVal;
}
/*============================================================================*/
float qFFMath_NextAfter( float x,
                         float y )
{
    float retVal;
    uint32_t ax, ay, uxi = 0U, uyi = 0U;
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( uxi, x, uint32_t );
    /*cppcheck-suppress misra-c2012-21.15 */
    cast_reinterpret( uyi, y, uint32_t );
    /*cstat -MISRAC2012-Rule-13.5*/
    if ( qFFMath_IsNaN( x ) || qFFMath_IsNaN( y ) ) {
    /*cstat +MISRAC2012-Rule-13.5*/
        retVal = QFFM_NAN;
    }
    else if ( uxi == uyi ) {
        retVal = y;
    }
    else {
        ax = uxi & 0x7FFFFFFFU;
        ay = uyi & 0x7FFFFFFFU;
        if ( 0U == ax ) {
            uxi = ( 0U == ay ) ? uyi : ( ( uyi & 0x80000000U ) | 1U );
        }
        else if ( ( ax > ay ) || ( 0U != ( ( uxi^uyi ) & 0x80000000U ) ) ) {
            uxi--;
        }
        else {
            uxi++;
        }
        /*cppcheck-suppress misra-c2012-21.15 */
        cast_reinterpret( retVal, uxi, float );
    }

    return retVal;
}
/*============================================================================*/
#endif /*#ifndef QLIBS_USE_STD_MATH*/