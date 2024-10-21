
// explicit template instantiation
template
struct Scalar<k, PassiveT, with_hessian>;

template
T1 pow( const T1 & a, const T2 & e);

template
PassiveT atan2( const PassiveT & _y, const PassiveT & _x);

template
PassiveT to_passive( const Scalar<k, PassiveT, with_hessian> & a);

template
Eigen::Matrix<ScalarT, rows, cols> to_passive( const Eigen::Matrix<Scalar<k, ScalarT, with_hessian>, rows, cols> & A);

// (dummy) function usage to encourage the compiler not to discard it
void INST_FUNC() {
    static constexpr int k_ = k;

    static constexpr bool with_hessian_ = with_hessian;

    static constexpr bool dynamic_mode_ = k == Eigen::Dynamic;

    using GradType = Eigen::Matrix<PassiveT, k, 1>;

    using HessType = typename std::conditional_t<
                with_hessian,
                Eigen::Matrix<PassiveT, k, k>,
                Eigen::Matrix<PassiveT, 0, 0>>;

    PassiveT *var0 = nullptr;
    const GradType *var1 = nullptr;
    const HessType *var2 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var3 = nullptr;
    cls_var3->known_derivatives( *var0, *var1, *var2 );

    PassiveT *var4 = nullptr;
    PassiveT *var5 = nullptr;
    PassiveT *var6 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var7 = nullptr;
    cls_var7->known_derivatives( *var4, *var5, *var6 );

    PassiveT *var8 = nullptr;
    Eigen::Index *var9 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var10 = nullptr;
    cls_var10->make_passive( *var8, *var9 );

    PassiveT *var11 = nullptr;
    Eigen::Index *var12 = nullptr;
    Eigen::Index *var13 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var14 = nullptr;
    cls_var14->make_active( *var11, *var12, *var13 );

    const Eigen::Matrix<PassiveT, Eigen::Dynamic, 1> *var15 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var16 = nullptr;
    cls_var16->make_active( *var15 );

    std::initializer_list<PassiveT> *var17 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var18 = nullptr;
    cls_var18->make_active( *var17 );

    const PassiveT *var19 = nullptr;
    const PassiveT *var20 = nullptr;
    const PassiveT *var21 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var22 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var23 = nullptr;
    cls_var23->chain( *var19, *var20, *var21, *var22 );

    const Scalar<k, PassiveT, with_hessian> *var24 = nullptr;
    operator-( *var24 );

    const Scalar<k, PassiveT, with_hessian> *var25 = nullptr;
    sqrt( *var25 );

    const Scalar<k, PassiveT, with_hessian> *var26 = nullptr;
    sqr( *var26 );

    const Scalar<k, PassiveT, with_hessian> *var27 = nullptr;
    const int *var28 = nullptr;
    pow( *var27, *var28 );

    const Scalar<k, PassiveT, with_hessian> *var29 = nullptr;
    const PassiveT *var30 = nullptr;
    pow( *var29, *var30 );

    const Scalar<k, PassiveT, with_hessian> *var31 = nullptr;
    fabs( *var31 );

    const Scalar<k, PassiveT, with_hessian> *var32 = nullptr;
    abs( *var32 );

    const Scalar<k, PassiveT, with_hessian> *var33 = nullptr;
    exp( *var33 );

    const Scalar<k, PassiveT, with_hessian> *var34 = nullptr;
    log( *var34 );

    const Scalar<k, PassiveT, with_hessian> *var35 = nullptr;
    log2( *var35 );

    const Scalar<k, PassiveT, with_hessian> *var36 = nullptr;
    log10( *var36 );

    const Scalar<k, PassiveT, with_hessian> *var37 = nullptr;
    sin( *var37 );

    const Scalar<k, PassiveT, with_hessian> *var38 = nullptr;
    cos( *var38 );

    const Scalar<k, PassiveT, with_hessian> *var39 = nullptr;
    tan( *var39 );

    const Scalar<k, PassiveT, with_hessian> *var40 = nullptr;
    asin( *var40 );

    const Scalar<k, PassiveT, with_hessian> *var41 = nullptr;
    acos( *var41 );

    const Scalar<k, PassiveT, with_hessian> *var42 = nullptr;
    atan( *var42 );

    const Scalar<k, PassiveT, with_hessian> *var43 = nullptr;
    sinh( *var43 );

    const Scalar<k, PassiveT, with_hessian> *var44 = nullptr;
    cosh( *var44 );

    const Scalar<k, PassiveT, with_hessian> *var45 = nullptr;
    tanh( *var45 );

    const Scalar<k, PassiveT, with_hessian> *var46 = nullptr;
    asinh( *var46 );

    const Scalar<k, PassiveT, with_hessian> *var47 = nullptr;
    acosh( *var47 );

    const Scalar<k, PassiveT, with_hessian> *var48 = nullptr;
    atanh( *var48 );

    const Scalar<k, PassiveT, with_hessian> *var49 = nullptr;
    isnan( *var49 );

    const Scalar<k, PassiveT, with_hessian> *var50 = nullptr;
    isinf( *var50 );

    const Scalar<k, PassiveT, with_hessian> *var51 = nullptr;
    isfinite( *var51 );

    const Scalar<k, PassiveT, with_hessian> *var52 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var53 = nullptr;
    operator+( *var52, *var53 );

    const Scalar<k, PassiveT, with_hessian> *var54 = nullptr;
    const PassiveT *var55 = nullptr;
    operator+( *var54, *var55 );

    const PassiveT *var56 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var57 = nullptr;
    operator+( *var56, *var57 );

    const Scalar<k, PassiveT, with_hessian> *var58 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var59 = nullptr;
    cls_var59->operator+=( *var58 );

    const PassiveT *var60 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var61 = nullptr;
    cls_var61->operator+=( *var60 );

    const Scalar<k, PassiveT, with_hessian> *var62 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var63 = nullptr;
    operator-( *var62, *var63 );

    const Scalar<k, PassiveT, with_hessian> *var64 = nullptr;
    const PassiveT *var65 = nullptr;
    operator-( *var64, *var65 );

    const PassiveT *var66 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var67 = nullptr;
    operator-( *var66, *var67 );

    const Scalar<k, PassiveT, with_hessian> *var68 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var69 = nullptr;
    cls_var69->operator-=( *var68 );

    const PassiveT *var70 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var71 = nullptr;
    cls_var71->operator-=( *var70 );

    const Scalar<k, PassiveT, with_hessian> *var72 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var73 = nullptr;
    operator*( *var72, *var73 );

    const Scalar<k, PassiveT, with_hessian> *var74 = nullptr;
    const PassiveT *var75 = nullptr;
    operator*( *var74, *var75 );

    const PassiveT *var76 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var77 = nullptr;
    operator*( *var76, *var77 );

    const Scalar<k, PassiveT, with_hessian> *var78 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var79 = nullptr;
    cls_var79->operator*=( *var78 );

    const PassiveT *var80 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var81 = nullptr;
    cls_var81->operator*=( *var80 );

    const Scalar<k, PassiveT, with_hessian> *var82 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var83 = nullptr;
    operator/( *var82, *var83 );

    const Scalar<k, PassiveT, with_hessian> *var84 = nullptr;
    const PassiveT *var85 = nullptr;
    operator/( *var84, *var85 );

    const PassiveT *var86 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var87 = nullptr;
    operator/( *var86, *var87 );

    const Scalar<k, PassiveT, with_hessian> *var88 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var89 = nullptr;
    cls_var89->operator/=( *var88 );

    const PassiveT *var90 = nullptr;
    Scalar<k, PassiveT, with_hessian> *cls_var91 = nullptr;
    cls_var91->operator/=( *var90 );

    const Scalar<k, PassiveT, with_hessian> *var92 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var93 = nullptr;
    atan2( *var92, *var93 );

    const Scalar<k, PassiveT, with_hessian> *var94 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var95 = nullptr;
    hypot( *var94, *var95 );

    const Scalar<k, PassiveT, with_hessian> *var96 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var97 = nullptr;
    operator==( *var96, *var97 );

    const Scalar<k, PassiveT, with_hessian> *var98 = nullptr;
    const PassiveT *var99 = nullptr;
    operator==( *var98, *var99 );

    const PassiveT *var100 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var101 = nullptr;
    operator==( *var100, *var101 );

    const Scalar<k, PassiveT, with_hessian> *var102 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var103 = nullptr;
    operator!=( *var102, *var103 );

    const Scalar<k, PassiveT, with_hessian> *var104 = nullptr;
    const PassiveT *var105 = nullptr;
    operator!=( *var104, *var105 );

    const PassiveT *var106 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var107 = nullptr;
    operator!=( *var106, *var107 );

    const Scalar<k, PassiveT, with_hessian> *var108 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var109 = nullptr;
    operator<( *var108, *var109 );

    const Scalar<k, PassiveT, with_hessian> *var110 = nullptr;
    const PassiveT *var111 = nullptr;
    operator<( *var110, *var111 );

    const PassiveT *var112 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var113 = nullptr;
    operator<( *var112, *var113 );

    const Scalar<k, PassiveT, with_hessian> *var114 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var115 = nullptr;
    operator<=( *var114, *var115 );

    const Scalar<k, PassiveT, with_hessian> *var116 = nullptr;
    const PassiveT *var117 = nullptr;
    operator<=( *var116, *var117 );

    const PassiveT *var118 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var119 = nullptr;
    operator<=( *var118, *var119 );

    const Scalar<k, PassiveT, with_hessian> *var120 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var121 = nullptr;
    operator>( *var120, *var121 );

    const Scalar<k, PassiveT, with_hessian> *var122 = nullptr;
    const PassiveT *var123 = nullptr;
    operator>( *var122, *var123 );

    const PassiveT *var124 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var125 = nullptr;
    operator>( *var124, *var125 );

    const Scalar<k, PassiveT, with_hessian> *var126 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var127 = nullptr;
    operator>=( *var126, *var127 );

    const Scalar<k, PassiveT, with_hessian> *var128 = nullptr;
    const PassiveT *var129 = nullptr;
    operator>=( *var128, *var129 );

    const PassiveT *var130 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var131 = nullptr;
    operator>=( *var130, *var131 );

    const Scalar<k, PassiveT, with_hessian> *var132 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var133 = nullptr;
    min( *var132, *var133 );

    const Scalar<k, PassiveT, with_hessian> *var134 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var135 = nullptr;
    fmin( *var134, *var135 );

    const Scalar<k, PassiveT, with_hessian> *var136 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var137 = nullptr;
    max( *var136, *var137 );

    const Scalar<k, PassiveT, with_hessian> *var138 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var139 = nullptr;
    fmax( *var138, *var139 );

    const Scalar<k, PassiveT, with_hessian> *var140 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var141 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var142 = nullptr;
    clamp( *var140, *var141, *var142 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var143 = nullptr;
    const std::complex<Scalar<k, PassiveT, with_hessian> > *var144 = nullptr;
    operator+( *var143, *var144 );

    const std::complex<PassiveT> *var145 = nullptr;
    const std::complex<Scalar<k, PassiveT, with_hessian> > *var146 = nullptr;
    operator+( *var145, *var146 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var147 = nullptr;
    const std::complex<PassiveT> *var148 = nullptr;
    operator+( *var147, *var148 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var149 = nullptr;
    const std::complex<Scalar<k, PassiveT, with_hessian> > *var150 = nullptr;
    operator-( *var149, *var150 );

    const std::complex<PassiveT> *var151 = nullptr;
    const std::complex<Scalar<k, PassiveT, with_hessian> > *var152 = nullptr;
    operator-( *var151, *var152 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var153 = nullptr;
    const std::complex<PassiveT> *var154 = nullptr;
    operator-( *var153, *var154 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var155 = nullptr;
    const std::complex<Scalar<k, PassiveT, with_hessian> > *var156 = nullptr;
    operator*( *var155, *var156 );

    const std::complex<PassiveT> *var157 = nullptr;
    const std::complex<Scalar<k, PassiveT, with_hessian> > *var158 = nullptr;
    operator*( *var157, *var158 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var159 = nullptr;
    const std::complex<PassiveT> *var160 = nullptr;
    operator*( *var159, *var160 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var161 = nullptr;
    sqr( *var161 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var162 = nullptr;
    const std::complex<Scalar<k, PassiveT, with_hessian> > *var163 = nullptr;
    operator/( *var162, *var163 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var164 = nullptr;
    const std::complex<PassiveT> *var165 = nullptr;
    operator/( *var164, *var165 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var166 = nullptr;
    conj( *var166 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var167 = nullptr;
    abs( *var167 );

    const std::complex<Scalar<k, PassiveT, with_hessian> > *var168 = nullptr;
    arg( *var168 );

    std::ostream *var169 = nullptr;
    const Scalar<k, PassiveT, with_hessian> *var170 = nullptr;
    operator<<( *var169, *var170 );
}

