/**
 * ANORRLTextOutputWidget.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

//this will make sure that we can use qt with boost (else moc compiler errors)
#ifndef QT_NO_KEYWORDS
    #define QT_NO_KEYWORDS
#endif

// Qt Headers
#include <QPlainTextEdit>
#include <QMenu>

// ANORRL Headers
#include "arl/signal.h"
#include "util/standardout.h"
#include "reflection/Property.h"

class QMenu;
class QAction;

// Override the plain text edit so that other features can be added to it.
class ANORRLTextOutputWidget : public QPlainTextEdit
{
    Q_OBJECT

    public:

        ANORRLTextOutputWidget(QWidget* parent);
        virtual ~ANORRLTextOutputWidget();

		virtual QSize sizeHint() const;

    protected:

        virtual void contextMenuEvent(QContextMenuEvent *event);
        virtual bool event(QEvent* evt);

		virtual void resizeEvent(QResizeEvent *e);

		bool isScrollOnBottom();

    protected Q_SLOTS:

        void appendOutputText(const QString message,ARL::MessageType type);

    private:

        void onPropertyChanged(const ARL::Reflection::PropertyDescriptor* pDescriptor);

        arl::signals::scoped_connection     m_PropertyChangedConnection;
        QTextCharFormat                     m_TextFormats[ARL::MESSAGE_TYPE_MAX];
};
