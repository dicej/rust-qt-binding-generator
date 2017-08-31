#include "Bindings.h"
#include "SortedModel.h"

#ifdef QT_CHARTS_LIB
#include <QtCharts>
#endif

#ifdef QT_QUICK_LIB
#include <QQmlApplicationEngine>
#include <QtQml/qqml.h>
#include <QQmlContext>
#include <QQuickView>
#include <QQuickItem>
#endif

#include <QApplication>
#include <QComboBox>
#include <QCommandLineParser>
#include <QDebug>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <QStyleFactory>
#include <QSvgRenderer>
#include <QSvgWidget>
#include <QTabWidget>
#include <QTableView>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWindow>

#include <cstdlib>

struct Model {
    QStringListModel styles;
    Demo demo;
    SortedModel sortedFileSystem;
    SortedModel sortedProcesses;
};

void setStyle(QWidget* w, QStyle* style) {
    for (QObject* o: w->children()) {
        QWidget* c = dynamic_cast<QWidget*>(o);
        if (c) {
            setStyle(c, style);
        }
    }
    w->setStyle(style);
}

QWindow* getWindow(QWidget* w) {
    QWidget* top = w;
    while (top && top->parentWidget()) {
        top = top->parentWidget();
    }
    return top->windowHandle();
}

#ifdef QT_QUICK_LIB

void copyWindowGeometry(QWidget* w, QQmlContext* c) {
    QWindow* window = getWindow(w);
    if (window) {
        c->setContextProperty("windowX", window->x());
        c->setContextProperty("windowY", window->y());
        c->setContextProperty("windowWidth", window->width());
        c->setContextProperty("windowHeight", window->height());
    }
}

void createQtQuick(const QString& name, const QString& qml, Model* model,
            QWidget* widgets, const QString& initialTab) {
    QQmlApplicationEngine* engine = new QQmlApplicationEngine();
    QQmlContext* c = engine->rootContext();
    c->setContextProperty("styles", &model->styles);
    c->setContextProperty("demo", &model->demo);
    c->setContextProperty("sortedFileSystem", &model->sortedFileSystem);
    c->setContextProperty("processes", &model->sortedProcesses);

    c->setContextProperty("widgets", widgets);
    c->setContextProperty("qtquickIndex",
        QVariant(model->styles.stringList().indexOf(name)));
    c->setContextProperty("initialTab", initialTab);
    copyWindowGeometry(widgets, engine->rootContext());
    engine->load(QUrl(qml));
}

#endif

QComboBox* createStyleComboBox(Model* model) {
    QComboBox* box = new QComboBox();
    box->setModel(&model->styles);
    auto styles = QStyleFactory::keys();
    QString currentStyle = QApplication::style()->objectName().toLower();
    for (auto v: styles) {
        box->addItem("QWidgets " + v);
        if (v.toLower() == currentStyle) {
            box->setCurrentText(v);
        }
    }
#ifdef QT_QUICK_LIB
    box->addItem("QtQuick");
#endif
#ifdef QTQUICKCONTROLS2
    box->addItem("QtQuick Controls 2");
#endif
    return box;
}

QWidget* createStyleTab(Model* model, QWidget* tabs, QComboBox* box,
        const QString& initialTab) {
    QRect windowRect;
    auto f = [windowRect, box, tabs, model, initialTab](const QString &text) mutable {
        QWindow* window = getWindow(tabs);
        bool visible = tabs->isVisible();
        if (text.startsWith("QWidgets ")) {
            tabs->setVisible(true);
            if (window && !visible) {
                window->setX(windowRect.x());
                window->setY(windowRect.y());
                window->setWidth(windowRect.width());
                window->setHeight(windowRect.height());
            }
            setStyle(tabs, QStyleFactory::create(text.mid(9)));
#ifdef QT_QUICK_LIB
        } else {
            if (window) {
                windowRect.setX(window->x());
                windowRect.setY(window->y());
                windowRect.setWidth(window->width());
                windowRect.setHeight(window->height());
            }
            tabs->setVisible(false);
#ifdef QTQUICKCONTROLS2
            if (text == "QtQuick Controls 2") {
                createQtQuick("QtQuick Controls 2", "qrc:///demo-qtquick2.qml",
                    model, box, initialTab);
            } else
#endif
            createQtQuick("QtQuick", "qrc:///demo.qml", model, box, initialTab);
#endif
        }
    };
    box->connect(box, &QComboBox::currentTextChanged, box, f);

    QSvgWidget* logo = new QSvgWidget(":/logo.svg");
    logo->setFixedSize(logo->renderer()->defaultSize());

    QWidget* tab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(box);
    layout->addWidget(logo);
    tab->setLayout(layout);
    return tab;
}

QWidget* createObjectTab(Model* model) {
    QWidget* view = new QWidget;
    Fibonacci* fibonacci = model->demo.fibonacci();

    QLineEdit* input = new QLineEdit;
    input->setPlaceholderText("Your number");
    input->setValidator(new QIntValidator(0, 100));
    input->connect(input, &QLineEdit::textChanged, fibonacci,
        [fibonacci](const QString& text) {
            fibonacci->setInput(text.toInt());
    });
    fibonacci->connect(fibonacci, &Fibonacci::inputChanged, input,
        [input, fibonacci]() {
            input->setText(QString::number(fibonacci->input()));
    });

    QLabel* label = new QLabel;
    fibonacci->connect(fibonacci, &Fibonacci::resultChanged, label,
        [label, fibonacci]() {
            label->setText("The Fibonacci number: "
                + QString::number(fibonacci->result()));
    });

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(input);
    layout->addWidget(label);
    view->setLayout(layout);
    return view;
}

QWidget* createListTab(Model* model) {
    QListView* view = new QListView();
    view->setModel(model->demo.fibonacciList());
    return view;
}

QWidget* createTreeTab(Model* model) {
    QTreeView* view = new QTreeView();
    view->setUniformRowHeights(true);
    view->setSortingEnabled(true);
    view->setModel(&model->sortedFileSystem);
    auto root = model->sortedFileSystem.index(0, 0);
    view->expand(root);
    view->sortByColumn(0, Qt::AscendingOrder);
    view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    return view;
}

QWidget* createProcessesTab(Model* model) {
    QTreeView* view = new QTreeView();
    view->setUniformRowHeights(true);
    view->setSortingEnabled(true);
    view->setModel(&model->sortedProcesses);
    // expand when the model is populated
    view->connect(&model->sortedProcesses, &QAbstractItemModel::rowsInserted,
        view, [view](const QModelIndex& index) {
        if (!index.isValid()) {
            view->expandAll();
        }
    });
    view->expandAll();
    view->sortByColumn(0, Qt::AscendingOrder);
    return view;
}

#ifdef QT_CHARTS_LIB

using namespace QtCharts;

QWidget* createChartTab(Model* model) {
    QLineSeries *series = new QLineSeries();
    series->setName("Line 1");
    QVXYModelMapper *mapper = new QVXYModelMapper(series);
    mapper->setXColumn(0);
    mapper->setYColumn(1);
    mapper->setSeries(series);
    mapper->setModel(model->demo.timeSeries());

    QChart* chart = new QChart;
    chart->addSeries(series);
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MMM yyyy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Sunspots count");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QWidget* view = new QWidget;
    QTableView* data = new QTableView;
    data->setModel(model->demo.timeSeries());
    QChartView *chartView = new QChartView(chart);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(data);
    layout->addWidget(chartView);
    view->setLayout(layout);
    return view;
}
#endif

void createWidgets(Model* model, const QString& initialStyle,
        const QString& initialTab) {
    QTabWidget* tabs = new QTabWidget();

    QComboBox* box = createStyleComboBox(model);
    tabs->addTab(createStyleTab(model, tabs, box, initialTab), "style");
    tabs->addTab(createObjectTab(model), "object");
    tabs->addTab(createListTab(model), "list");
    tabs->addTab(createTreeTab(model), "tree");
    const int procTab = tabs->addTab(createProcessesTab(model), "processes");
    tabs->connect(tabs, &QTabWidget::currentChanged, model->demo.processes(),
        [model, procTab](int current) {
            model->demo.processes()->setActive(current == procTab);
        });
#ifdef QT_CHARTS_LIB
    tabs->addTab(createChartTab(model), "chart");
#endif
    tabs->setMinimumSize(QSize(500, 500));
    tabs->show();
    box->setCurrentText(initialStyle);
    for (int i = 0; i < tabs->count(); ++i) {
        if (tabs->tabText(i) == initialTab) {
            tabs->setCurrentIndex(i);
        }
    }
}

int main (int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/logo.svg"));

#ifdef QT_QUICK_LIB
    qmlRegisterType<QSortFilterProxyModel>("org.qtproject.example", 1, 0, "SortFilterProxyModel");
    qmlRegisterType<Fibonacci>("rust", 1, 0, "Fibonacci");
    qmlRegisterType<FibonacciList>("rust", 1, 0, "FibonacciList");
#endif

    QCommandLineParser parser;
    parser.setApplicationDescription("Demo application for Qt and RUst");
    parser.addHelpOption();
    parser.addVersionOption();
    // --initial-style
    QCommandLineOption initialStyleOption(QStringList()
            << "initial-style",
            QCoreApplication::translate("main", "Initial widget style."),
            QCoreApplication::translate("main", "style"));
    parser.addOption(initialStyleOption);
    // --initial-tab
    QCommandLineOption initialTabOption(QStringList()
            << "initial-tab",
            QCoreApplication::translate("main", "Initial tab."),
            QCoreApplication::translate("main", "tab"));
    parser.addOption(initialTabOption);
    parser.process(app);

    Model model;
    model.demo.fileSystemTree()->setPath("/");
    model.sortedFileSystem.setSourceModel(model.demo.fileSystemTree());
    model.sortedFileSystem.setDynamicSortFilter(true);
    model.sortedProcesses.setSourceModel(model.demo.processes());
    model.sortedProcesses.setDynamicSortFilter(true);

    createWidgets(&model, parser.value(initialStyleOption),
                           parser.value(initialTabOption));

    return app.exec();
}
