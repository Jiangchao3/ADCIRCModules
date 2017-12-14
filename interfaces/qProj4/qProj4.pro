
TARGET = qproj4
TEMPLATE = lib
CONFIG += static
CONFIG -= qt

#...PROJ4 Library
PROJPATH      = $$PWD/../../thirdparty/proj4/src

#...PROJ Sources
SOURCES_PROJ4 = \
    $$PROJPATH/pj_list.h $$PROJPATH/PJ_aeqd.c $$PROJPATH/PJ_gnom.c $$PROJPATH/PJ_laea.c $$PROJPATH/PJ_mod_ster.c \
    $$PROJPATH/PJ_nsper.c $$PROJPATH/PJ_nzmg.c $$PROJPATH/PJ_ortho.c $$PROJPATH/PJ_stere.c $$PROJPATH/PJ_sterea.c \
    $$PROJPATH/PJ_aea.c $$PROJPATH/PJ_bipc.c $$PROJPATH/PJ_bonne.c $$PROJPATH/PJ_eqdc.c $$PROJPATH/PJ_isea.c \
    $$PROJPATH/PJ_imw_p.c $$PROJPATH/PJ_krovak.c $$PROJPATH/PJ_lcc.c $$PROJPATH/PJ_poly.c $$PROJPATH/PJ_rpoly.c \
    $$PROJPATH/PJ_sconics.c $$PROJPATH/proj_rouss.c $$PROJPATH/PJ_cass.c $$PROJPATH/PJ_cc.c $$PROJPATH/PJ_cea.c \
    $$PROJPATH/PJ_eqc.c $$PROJPATH/PJ_gall.c $$PROJPATH/PJ_labrd.c $$PROJPATH/PJ_lsat.c $$PROJPATH/PJ_merc.c \
    $$PROJPATH/PJ_mill.c $$PROJPATH/PJ_ocea.c $$PROJPATH/PJ_omerc.c $$PROJPATH/PJ_somerc.c $$PROJPATH/PJ_tcc.c \
    $$PROJPATH/PJ_tcea.c $$PROJPATH/PJ_tmerc.c $$PROJPATH/PJ_airy.c $$PROJPATH/PJ_aitoff.c $$PROJPATH/PJ_august.c \
    $$PROJPATH/PJ_bacon.c $$PROJPATH/PJ_chamb.c $$PROJPATH/PJ_hammer.c $$PROJPATH/PJ_lagrng.c $$PROJPATH/PJ_larr.c \
    $$PROJPATH/PJ_lask.c $$PROJPATH/PJ_nocol.c $$PROJPATH/PJ_ob_tran.c $$PROJPATH/PJ_oea.c $$PROJPATH/PJ_tpeqd.c \
    $$PROJPATH/PJ_vandg.c $$PROJPATH/PJ_vandg2.c $$PROJPATH/PJ_vandg4.c $$PROJPATH/PJ_wag7.c $$PROJPATH/PJ_lcca.c \
    $$PROJPATH/PJ_geos.c $$PROJPATH/proj_etmerc.c $$PROJPATH/PJ_boggs.c $$PROJPATH/PJ_collg.c $$PROJPATH/PJ_crast.c \
    $$PROJPATH/PJ_denoy.c $$PROJPATH/PJ_eck1.c $$PROJPATH/PJ_eck2.c $$PROJPATH/PJ_eck3.c $$PROJPATH/PJ_eck4.c \
    $$PROJPATH/PJ_eck5.c $$PROJPATH/PJ_fahey.c $$PROJPATH/PJ_fouc_s.c $$PROJPATH/PJ_gins8.c $$PROJPATH/PJ_gstmerc.c \
    $$PROJPATH/PJ_gn_sinu.c $$PROJPATH/PJ_goode.c $$PROJPATH/PJ_igh.c $$PROJPATH/PJ_hatano.c $$PROJPATH/PJ_loxim.c \
    $$PROJPATH/PJ_mbt_fps.c $$PROJPATH/PJ_mbtfpp.c $$PROJPATH/PJ_mbtfpq.c $$PROJPATH/PJ_moll.c $$PROJPATH/PJ_nell.c \
    $$PROJPATH/PJ_nell_h.c $$PROJPATH/PJ_putp2.c $$PROJPATH/PJ_putp3.c $$PROJPATH/PJ_putp4p.c $$PROJPATH/PJ_putp5.c \
    $$PROJPATH/PJ_putp6.c $$PROJPATH/PJ_qsc.c $$PROJPATH/PJ_robin.c $$PROJPATH/PJ_sts.c $$PROJPATH/PJ_urm5.c \
    $$PROJPATH/PJ_urmfps.c $$PROJPATH/PJ_wag2.c $$PROJPATH/PJ_wag3.c $$PROJPATH/PJ_wink1.c $$PROJPATH/PJ_wink2.c \
    $$PROJPATH/pj_latlong.c $$PROJPATH/pj_geocent.c $$PROJPATH/aasincos.c $$PROJPATH/adjlon.c $$PROJPATH/bch2bps.c \
    $$PROJPATH/bchgen.c $$PROJPATH/biveval.c $$PROJPATH/dmstor.c $$PROJPATH/mk_cheby.c $$PROJPATH/pj_auth.c \
    $$PROJPATH/pj_deriv.c $$PROJPATH/pj_ell_set.c $$PROJPATH/pj_ellps.c $$PROJPATH/pj_errno.c $$PROJPATH/pj_factors.c \
    $$PROJPATH/pj_fwd.c $$PROJPATH/pj_init.c $$PROJPATH/pj_inv.c $$PROJPATH/pj_list.c $$PROJPATH/pj_malloc.c \
    $$PROJPATH/pj_mlfn.c $$PROJPATH/pj_msfn.c $$PROJPATH/proj_mdist.c $$PROJPATH/pj_open_lib.c $$PROJPATH/pj_param.c \
    $$PROJPATH/pj_phi2.c $$PROJPATH/pj_pr_list.c $$PROJPATH/pj_qsfn.c $$PROJPATH/pj_strerrno.c $$PROJPATH/pj_tsfn.c \
    $$PROJPATH/pj_units.c $$PROJPATH/pj_ctx.c $$PROJPATH/pj_log.c $$PROJPATH/pj_zpoly1.c $$PROJPATH/rtodms.c \
    $$PROJPATH/vector1.c $$PROJPATH/pj_release.c $$PROJPATH/pj_gauss.c $$PROJPATH/PJ_healpix.c $$PROJPATH/PJ_natearth.c \
    $$PROJPATH/PJ_calcofi.c $$PROJPATH/pj_fileapi.c $$PROJPATH/pj_gc_reader.c $$PROJPATH/pj_gridcatalog.c \
    $$PROJPATH/nad_cvt.c $$PROJPATH/nad_init.c $$PROJPATH/nad_intr.c $$PROJPATH/emess.c $$PROJPATH/emess.h \
    $$PROJPATH/pj_apply_gridshift.c $$PROJPATH/pj_datums.c $$PROJPATH/pj_datum_set.c $$PROJPATH/pj_transform.c \
    $$PROJPATH/geocent.c $$PROJPATH/geocent.h $$PROJPATH/pj_utils.c $$PROJPATH/pj_gridinfo.c $$PROJPATH/pj_gridlist.c \
    $$PROJPATH/jniproj.c $$PROJPATH/pj_mutex.c $$PROJPATH/pj_initcache.c $$PROJPATH/pj_apply_vgridshift.c $$PROJPATH/geodesic.c \
    $$PROJPATH/pj_strtod.c

HEADERS += epsg.h \
           qproj4.h

SOURCES += $$SOURCES_PROJ4 qproj4.cpp

INCLUDEPATH += $$PROJPATH

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../point/release/ -lpoint
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../point/debug/ -lpoint
else:unix: LIBS += -L$$OUT_PWD/../point/ -lpoint

INCLUDEPATH += $$PWD/../point
DEPENDPATH += $$PWD/../point

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../point/release/libpoint.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../point/debug/libpoint.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../point/release/point.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../point/debug/point.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../point/libpoint.a
