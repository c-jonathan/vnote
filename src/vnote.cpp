#include <QSettings>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include "vnote.h"
#include "utils/vutils.h"
#include "vconfigmanager.h"

VConfigManager vconfig;

QString VNote::templateHtml;
QString VNote::preTemplateHtml;
QString VNote::postTemplateHtml;

VNote::VNote(QObject *parent)
    : QObject(parent)
{
    vconfig.initialize();
    initTemplate();
    vconfig.getNotebooks(m_notebooks, this);
}

void VNote::initPalette(QPalette palette)
{
    m_palette.clear();

    m_palette.append(QPair<QString, QString>("base-background",
                                             palette.background().color().name()));
    m_palette.append(QPair<QString, QString>("base-foreground",
                                             palette.background().color().name()));
    m_palette.append(QPair<QString, QString>("hover-color", "#42A5F5"));
    m_palette.append(QPair<QString, QString>("base-color", "#81C784"));

    // Material Design Colors
    m_palette.append(QPair<QString, QString>("teal0", "#E0F2F1"));
    m_palette.append(QPair<QString, QString>("teal1", "#B2DFDB"));
    m_palette.append(QPair<QString, QString>("teal2", "#80CBC4"));
    m_palette.append(QPair<QString, QString>("teal3", "#4DB6AC"));
    m_palette.append(QPair<QString, QString>("teal4", "#26A69A"));

    m_palette.append(QPair<QString, QString>("Indigo0", "#E8EAF6"));
    m_palette.append(QPair<QString, QString>("Indigo1", "#C5CAE9"));
    m_palette.append(QPair<QString, QString>("Indigo2", "#9FA8DA"));
    m_palette.append(QPair<QString, QString>("Indigo3", "#7986CB"));
    m_palette.append(QPair<QString, QString>("Indigo4", "#5C6BC0"));
}

void VNote::initTemplate()
{
    if (templateHtml.isEmpty() || preTemplateHtml.isEmpty()
        || postTemplateHtml.isEmpty()) {
        updateTemplate();
    }
}

void VNote::updateTemplate()
{
    // Get background color
    QString rgb;
    const QString &curRenderBg = vconfig.getCurRenderBackgroundColor();
    const QVector<VColor> &predefinedColors = vconfig.getPredefinedColors();
    if (curRenderBg != "System") {
        for (int i = 0; i < predefinedColors.size(); ++i) {
            if (predefinedColors[i].name == curRenderBg) {
                rgb = predefinedColors[i].rgb;
                break;
            }
        }
    }
    QString cssStyle;
    if (!rgb.isEmpty()) {
        cssStyle = "body { background-color: #" + rgb + "; }";
    }
    QString styleHolder("<!-- BACKGROUND_PLACE_HOLDER -->");
    QString cssHolder("CSS_PLACE_HOLDER");
    templateHtml = VUtils::readFileFromDisk(vconfig.getTemplatePath());
    templateHtml.replace(cssHolder, vconfig.getTemplateCssUrl());
    if (!cssStyle.isEmpty()) {
        templateHtml.replace(styleHolder, cssStyle);
    }

    preTemplateHtml = VUtils::readFileFromDisk(vconfig.getPreTemplatePath());
    preTemplateHtml.replace(cssHolder, vconfig.getTemplateCssUrl());
    if (!cssStyle.isEmpty()) {
        preTemplateHtml.replace(styleHolder, cssStyle);
    }

    postTemplateHtml = VUtils::readFileFromDisk(vconfig.getPostTemplatePath());
}

const QVector<VNotebook *> &VNote::getNotebooks() const
{
    return m_notebooks;
}

QVector<VNotebook *> &VNote::getNotebooks()
{
    return m_notebooks;
}