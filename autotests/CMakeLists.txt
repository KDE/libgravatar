# SPDX-FileCopyrightText: none
# SPDX-License-Identifier: BSD-3-Clause
macro(add_gravatar_unittest _source)
    set( _test ${_source})
    get_filename_component( _name ${_source} NAME_WE )
    add_executable( ${_name} ${_test} ${_name}.h)
    add_test(NAME ${_name} COMMAND ${_name} )
    ecm_mark_as_test(${_name})
    target_link_libraries( ${_name} KPim${KF_MAJOR_VERSION}::PimCommon Qt::Test KPim${KF_MAJOR_VERSION}::Gravatar KF${KF_MAJOR_VERSION}::TextWidgets)
endmacro ()

add_gravatar_unittest( gravatarcachetest.cpp )
add_gravatar_unittest( gravatarconfiguresettingsdialogtest.cpp )
add_gravatar_unittest( gravatardownloadpixmapdialogtest.cpp )
add_gravatar_unittest( gravatardownloadpixmapwidgettest.cpp )
add_gravatar_unittest( gravatarresolvurljobtest.cpp )
add_gravatar_unittest( gravatarconfiguresettingswidgettest.cpp )

