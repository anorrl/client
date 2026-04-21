/**
 * ScriptSyntaxHighlighter.h
 * Copyright (c) 2013 ROBLOX Corp. All Rights Reserved.
 */

#pragma once

//this will make sure that we can use qt with boost (else moc compiler errors)
#ifndef QT_NO_KEYWORDS
    #define QT_NO_KEYWORDS
#endif

// Qt Headers
#include <QSyntaxHighlighter>

// Roblox Headers
#include "reflection/Property.h"

class QTextDocument;
class QStringList;
class QTextCharFormat;

enum ARLLuaLexState
{
    ARL_LUA_DEFAULT,
    ARL_LUA_COMMENT,
    ARL_LUA_COMMENTLINE,
    ARL_LUA_NUMBER,
    ARL_LUA_WORD,
    ARL_LUA_STRING,
    ARL_LUA_CHARACTER,
    ARL_LUA_LITERALSTRING,
    ARL_LUA_PREPROCESSOR,
    ARL_LUA_OPERATOR,
    ARL_LUA_IDENTIFIER,
    ARL_LUA_STRINGEOL
};

class ScriptSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:

    ScriptSyntaxHighlighter(QTextDocument* parent = 0);

    void onPropertyChanged(const ARL::Reflection::PropertyDescriptor* pDescriptor);
    void setFont(const QFont& font);

protected:

    void highlightBlock(const QString& text);

private:

    void initData();

    void setLexState(
        ARLLuaLexState lexState,
        int            currentPos);

    bool checkApplyFoldState(const QString& keyword);

    QStringList     m_keywordPatterns;
    QStringList     m_foldStarts;
    QStringList     m_foldEnds;

    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_operatorFormat;
    QTextCharFormat m_numberFormat;
    QTextCharFormat m_commentFormat;
    QTextCharFormat m_stringFormat;
    QTextCharFormat m_preprocessorFormat;
    QTextCharFormat m_defaultFormat;

    int             m_startSegPos;
    int             m_nestLevel;
    int             m_sepCount;

    ARLLuaLexState m_lexState;
};
