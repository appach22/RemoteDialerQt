#!/bin/bash

install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $1.app/Contents/Frameworks/QtCore.framework/Versions/5/QtCore
install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui $1.app/Contents/Frameworks/QtGui.framework/Versions/5/QtGui
install_name_tool -id @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets $1.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets
install_name_tool -id @executable_path/../Frameworks/QtNetwork.framework/Versions/5/QtNetwork $1.app/Contents/Frameworks/QtNetwork.framework/Versions/5/QtNetwork

install_name_tool -change /Users/appach/Qt5.0.2/5.0.2/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $1.app/Contents/MacOS/$1
install_name_tool -change /Users/appach/Qt5.0.2/5.0.2/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui $1.app/Contents/MacOS/$1
install_name_tool -change /Users/appach/Qt5.0.2/5.0.2/clang_64/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets $1.app/Contents/MacOS/$1
install_name_tool -change /Users/appach/Qt5.0.2/5.0.2/clang_64/lib/QtNetwork.framework/Versions/5/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/5/QtNetwork $1.app/Contents/MacOS/$1

install_name_tool -change /Users/appach/Qt5.0.2/5.0.2/clang_64/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui $1.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets
install_name_tool -change /Users/appach/Qt5.0.2/5.0.2/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $1.app/Contents/Frameworks/QtWidgets.framework/Versions/5/QtWidgets
install_name_tool -change /Users/appach/Qt5.0.2/5.0.2/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $1.app/Contents/Frameworks/QtGui.framework/Versions/5/QtGui
install_name_tool -change /Users/appach/Qt5.0.2/5.0.2/clang_64/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore $1.app/Contents/Frameworks/QtNetwork.framework/Versions/5/QtNetwork

