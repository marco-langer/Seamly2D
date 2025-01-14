# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

SOURCES += \
    $$PWD/variables/custom_variable.cpp \
    $$PWD/variables/measurement_variable.cpp \
    $$PWD/vcontainer.cpp \
    $$PWD/calculator.cpp \
    $$PWD/vnodedetail.cpp \
    $$PWD/vtranslatevars.cpp \
    $$PWD/variables/varcradius.cpp \
    $$PWD/variables/vcurveangle.cpp \
    $$PWD/variables/vcurvelength.cpp \
    $$PWD/variables/vcurvevariable.cpp \
    $$PWD/variables/vinternalvariable.cpp \
    $$PWD/variables/vlineangle.cpp \
    $$PWD/variables/vlinelength.cpp \
    $$PWD/variables/vvariable.cpp \
    $$PWD/vformula.cpp \
    $$PWD/variables/vcurveclength.cpp \
    $$PWD/vpiece.cpp \
    $$PWD/vpiecenode.cpp \
    $$PWD/vpiecepath.cpp \
    $$PWD/floatItemData/vpiecelabeldata.cpp \
    $$PWD/floatItemData/vpatternlabeldata.cpp \
    $$PWD/floatItemData/vgrainlinedata.cpp \
    $$PWD/floatItemData/vabstractfloatitemdata.cpp \
    $$PWD/measurements_def.cpp \
    $$PWD/pmsystems.cpp

*msvc*:SOURCES += $$PWD/stable.cpp

HEADERS += \
    $$PWD/variables/custom_variable.h \
    $$PWD/variables/custom_variable_p.h \
    $$PWD/variables/measurement_variable.h \
    $$PWD/variables/measurement_variable_p.h \
    $$PWD/vcontainer.h \
    $$PWD/stable.h \
    $$PWD/calculator.h \
    $$PWD/variables.h \
    $$PWD/vnodedetail.h \
    $$PWD/vtranslatevars.h \
    $$PWD/variables/varcradius.h \
    $$PWD/variables/varcradius_p.h \
    $$PWD/variables/vcurveangle.h \
    $$PWD/variables/vcurvelength.h \
    $$PWD/variables/vcurvevariable.h \
    $$PWD/variables/vcurvevariable_p.h \
    $$PWD/variables/vinternalvariable.h \
    $$PWD/variables/vinternalvariable_p.h \
    $$PWD/variables/vlineangle.h \
    $$PWD/variables/vlineangle_p.h \
    $$PWD/variables/vlinelength.h \
    $$PWD/variables/vlinelength_p.h \
    $$PWD/variables/vvariable.h \
    $$PWD/variables/vvariable_p.h \
    $$PWD/vformula.h \
    $$PWD/variables/vcurveclength.h \
    $$PWD/vpiece.h \
    $$PWD/vpiece_p.h \
    $$PWD/vpiecenode.h \
    $$PWD/vpiecenode_p.h \
    $$PWD/vpiecepath.h \
    $$PWD/vpiecepath_p.h \
    $$PWD/floatItemData/vpiecelabeldata.h \
    $$PWD/floatItemData/vpatternlabeldata.h \
    $$PWD/floatItemData/vgrainlinedata.h \
    $$PWD/floatItemData/vabstractfloatitemdata.h \
    $$PWD/floatItemData/vabstractfloatitemdata_p.h \
    $$PWD/floatItemData/vgrainlinedata_p.h \
    $$PWD/floatItemData/floatitemdef.h \
    $$PWD/floatItemData/vpatternlabeldata_p.h \
    $$PWD/floatItemData/vpiecelabeldata_p.h \
    $$PWD/measurements_def.h \
    $$PWD/pmsystems.h
