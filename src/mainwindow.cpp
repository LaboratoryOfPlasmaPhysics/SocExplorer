/*------------------------------------------------------------------------------
--  This file is a part of the SocExplorer Software
--  Copyright (C) 2011, Laboratory of Plasmas Physic - CNRS
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
-------------------------------------------------------------------------------*/
/*--                  Author : Alexis Jeandet
--                     Mail : alexis.jeandet@lpp.polytechnique.fr
----------------------------------------------------------------------------*/
#include "mainwindow.h"
#include <QDockWidget>

LPMONMainWindow::LPMONMainWindow(QString ScriptToEval, QWidget *parent)
    : QMainWindow(parent)
{
    QCoreApplication::setApplicationName("SocExplorer");
    QCoreApplication::setOrganizationName("LPP");
    QCoreApplication::setOrganizationDomain("lpp.fr");
    this->makeObjects(ScriptToEval);
    this->makeLayout();
    this->makeMenu();
    this->makeConnections();
    this->setWindowIcon(QIcon(tr(":/images/logolppcutted.png")));
    this->setAcceptDrops(true);
    this->pluginManager->setRootLoadable(true);
    this->PythonConsoleInst->pyConsoleRunFile(ScriptToEval);
    QProgressBar* test = SocExplorerEngine::getProgressBar("test",10);
    statusBar()->setFixedHeight(statusBar()->height());
    SocExplorerEngine::deleteProgressBar(test);
}


void LPMONMainWindow::makeObjects(QString ScriptToEval)
{
    Q_UNUSED(ScriptToEval)
    pluginsDockContainer = new QMainWindow;
    pluginsDockContainer->setWindowFlags(Qt::Widget);
    pluginsDockContainer->setDockNestingEnabled(true);
    this->mainWidget = new QSplitter(Qt::Vertical);
    this->appTranslator = new QTranslator;
    this->Quit = new QAction(tr("&Quit"),this);
    this->Quit->setShortcut(tr("CTRL+Q"));
    this->ManagePlugins = new QAction(tr("&Manage Plugins"),this);
    this->ManagePlugins->setShortcut(tr("CTRL+P"));
    this->regsManager = new QAction(tr("&Manage registers"),this);
    this->exploreRegs = new QAction(tr("&Explore registers"),this);
    this->help = new QAction(tr("&Help"),this);
    this->help->setShortcut(tr("CTRL+H"));
    this->about = new QAction(tr("&About"),this);
    socexplorerproxy::setMainWindow(this);
    SocExplorerEngine::setMainWindow(this);
    SocExplorerEngine::xmlModel()->scanXmlFiles();
    this->regExplorer = new regsExplorer();
    this->regExplorer->setAllowedAreas(Qt::AllDockWidgetAreas);
    this->pluginsDockContainer->addDockWidget(Qt::TopDockWidgetArea,this->regExplorer);
    this->PythonConsoleInst = new PythonConsole(socexplorerproxy::self());
    this->PythonConsoleInst->addObject("SocExplorerEngine",SocExplorerEngine::self());
    this->pluginManager = new dockablePluginManager();
    this->toolpane = new toolBar;
    this->p_about = new aboutsocexplorer();
}

void LPMONMainWindow::makeLayout()
{
    this->mainWidget->addWidget(pluginsDockContainer);
    this->mainWidget->addWidget(this->PythonConsoleInst);
    this->toolpane->setAllowedAreas(Qt::RightDockWidgetArea|Qt::LeftDockWidgetArea);
    this->addDockWidget(Qt::LeftDockWidgetArea,this->toolpane);
    this->toolpane->addTool(this->pluginManager);
    this->setCentralWidget(this->mainWidget);
}


void LPMONMainWindow::makeConnections()
{
    connect(socexplorerproxy::self(),SIGNAL(clearMenu()),this,SLOT(clearMenu()));
    connect(this,SIGNAL(translateSig()),socexplorerproxy::self(),SLOT(updateText()));
    connect(socexplorerproxy::self(),SIGNAL(addPluginGUI(QDockWidget*)),this,SLOT(addPluginInterface(QDockWidget*)));
    connect(this->ManagePlugins,SIGNAL(triggered()),this,SLOT(launchPluginManager()));
    connect(this->Quit,SIGNAL(triggered()),qApp,SLOT(quit()));
    connect(this,SIGNAL(registerObject(QObject*,QString)),this->PythonConsoleInst,SLOT(registerObject(QObject*,QString)));
    connect(socexplorerproxy::self(),SIGNAL(registerObject(QObject*,QString)),this,SIGNAL(registerObject(QObject*,QString)));
    connect(this->pluginManager,SIGNAL(geteplugintree()),socexplorerproxy::self(),SLOT(geteplugintree()));
    connect(socexplorerproxy::self(),SIGNAL(treeChanged(QList<socexplorerplugin*>)),this->pluginManager,SIGNAL(treeChanged(QList<socexplorerplugin*>)));
    connect(this->pluginManager,SIGNAL(changeSysDriverInstName(QString,QString)),socexplorerproxy::self(),SLOT(changeSysDriverInstName(QString,QString)));
    connect(this->pluginManager,SIGNAL(changeSysDriverInstName(QString,QString)),this->PythonConsoleInst,SLOT(changeSysDriverInstName(QString,QString)));
    connect(this->pluginManager,SIGNAL(closeSysDriver(QString)),socexplorerproxy::self(),SLOT(closeSysDriver(QString)));
    connect(this->pluginManager,SIGNAL(closeSysDriver(QString)),this->PythonConsoleInst,SLOT(removeDriver(QString)));
    connect(this->about,SIGNAL(triggered()),this,SLOT(showAboutBox()));
    connect(this->exploreRegs,SIGNAL(triggered()),this->regExplorer,SLOT(show()));
    this->pluginManager->connect(this->pluginManager,SIGNAL(loadSysDrviver(QString)),socexplorerproxy::self(),SLOT(loadSysDriver(QString)));
    this->pluginManager->connect(this->pluginManager,SIGNAL(loadSysDriverToParent(QString,QString)),socexplorerproxy::self(),SLOT(loadSysDriverToParent(QString,QString)));

}


void LPMONMainWindow::launchPluginManager()
{

    if(this->pluginManager->isHidden())
    {
        this->pluginManager->setHidden(false);
    }

}


void LPMONMainWindow::addPluginInterface(QDockWidget *plugin)
{
    static QDockWidget* last=NULL;
    plugin->setAllowedAreas(Qt::AllDockWidgetAreas);
    this->pluginsDockContainer->addDockWidget(Qt::TopDockWidgetArea,plugin);
    if(last!=NULL)
        this->pluginsDockContainer->tabifyDockWidget(last,plugin);
    last = plugin;

}


void LPMONMainWindow::clearMenu()
{
    this->menuBar()->clear();
    this->makeMenu();
}


void LPMONMainWindow::makeMenu()
{
    this->FileMenu = menuBar()->addMenu(tr("&File"));
    this->PluginsMenu = menuBar()->addMenu(tr("&Plugins"));
    this->ToolsMenu = menuBar()->addMenu(tr("&Tools"));
    this->ToolsMenu->addAction(this->exploreRegs);
    this->FileMenu->addAction(this->Quit);
    socexplorerproxy::self()->makeMenu(this->PluginsMenu);
    this->PluginsMenu->addAction(this->ManagePlugins);

    this->helpMenu = menuBar()->addMenu(tr("Help"));
    this->helpMenu->addAction(this->help);
    this->helpMenu->addAction(this->about);

}


LPMONMainWindow::~LPMONMainWindow()
{
    delete this->p_about;
}


void LPMONMainWindow::setLangage(QAction *action)
{
    QString local = action->data().toString();
    QString qmPath = QDir(QString("translations")).absolutePath();
    appTranslator->load(qmPath+"/socexplorer_"+local+".qm");
    qApp->installTranslator(appTranslator);
    emit this->translateSig();
}


void LPMONMainWindow::createLangMenu()
{
    this->langMenu = menuBar()->addMenu(tr("&Langue"));
    this->langActionGrp = new QActionGroup(this);
    connect(this->langActionGrp,SIGNAL(triggered(QAction*)),this,SLOT(setLangage(QAction*)));
    QDir* qmDir = new QDir(QString("translations"));
    QStringList LangFiles = qmDir->entryList(QStringList("socexplorer_*.qm"));
    for(int i=0;i<LangFiles.size();++i)
    {
        QString Local = LangFiles[i];
        Local.remove(0,Local.indexOf('_')+1);
        Local.chop(3);
        QTranslator translator;
        translator.load(LangFiles[i],qmDir->absolutePath());
        QString langage = translator.translate("MainWindow","English");
        QAction *action = new QAction(tr("&%1 %2").arg(i+1).arg(langage),this);
        action->setCheckable(true);
        action->setData(Local);
        langMenu->addAction(action);
        langActionGrp->addAction(action);
        if(langage==tr("English"))
            action->setChecked(true);
    }
}


void LPMONMainWindow::updateText()
{
    emit this->translateSig();
}



void LPMONMainWindow::showAboutBox()
{
    p_about->show();
}



void LPMONMainWindow::closeEvent(QCloseEvent *event)
{
    socexplorerproxy::self()->close();
    qApp->closeAllWindows();
    event->accept();
}





