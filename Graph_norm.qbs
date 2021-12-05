import qbs.FileInfo

QtApplication {
    Depends { name: "Qt.widgets" }
    Depends { name: "Qt.printsupport" }
    Depends { name: "Qt.network" }

    cpp.defines: [
        // You can make your code fail to compile if it uses deprecated APIs.
        // In order to do so, uncomment the following line.
        //"QT_DISABLE_DEPRECATED_BEFORE=0x060000" // disables all the APIs deprecated before Qt 6.0.0
    ]

    files: [
        "main.cpp",
        "mainwindow.cpp",
        "mainwindow.h",
        "mainwindow.ui",
        "parser.cpp",
        "parser.h",
        "qcustomplot.cpp",
        "qcustomplot.h",
        "requester.cpp",
        "requester.h",
    ]

    install: true
    installDir: qbs.targetOS.contains("qnx") ? FileInfo.joinPaths("/tmp", name, "bin") : base
}
