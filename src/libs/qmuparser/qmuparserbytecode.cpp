/***************************************************************************
 **  @file   qmuparserbytecode.cpp
 **  @author Douglas S Caskey
 **  @date   17 Sep, 2023
 **
 **  @copyright
 **  Copyright (C) 2017 - 2023 Seamly, LLC
 **  https://github.com/fashionfreedom/seamly2d
 **
 **  @brief
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Seamly2D is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D. If not, see <http://www.gnu.org/licenses/>.
 **************************************************************************/

/***************************************************************************************************
 **
 **  Copyright (C) 2013 Ingo Berg
 **
 **  Permission is hereby granted, free of charge, to any person obtaining a copy of this
 **  software and associated documentation files (the "Software"), to deal in the Software
 **  without restriction, including without limitation the rights to use, copy, modify,
 **  merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 **  permit persons to whom the Software is furnished to do so, subject to the following conditions:
 **
 **  The above copyright notice and this permission notice shall be included in all copies or
 **  substantial portions of the Software.
 **
 **  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *BUT
 **  NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 **  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 **  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 **  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **
 ******************************************************************************************************/

#include "qmuparserbytecode.h"

#include <QMessageLogger>
#include <QStack>
#include <QString>
#include <QtDebug>
#include <QtMath>
#include <assert.h>

#include "qmudef.h"
#include "qmuparsererror.h"

namespace qmu {
//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Bytecode default constructor.
 */
QmuParserByteCode::QmuParserByteCode()
    : m_iStackPos(0)
    , m_iMaxStackSize(0)
    , m_vRPN()
    , m_bEnableOptimizer(true)
{
    m_vRPN.reserve(50);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Copy constructor.
 *
 * Implemented in Terms of Assign(const QParserByteCode &a_ByteCode)
 */
QmuParserByteCode::QmuParserByteCode(const QmuParserByteCode& a_ByteCode)
    : m_iStackPos(a_ByteCode.m_iStackPos)
    , m_iMaxStackSize(a_ByteCode.m_iMaxStackSize)
    , m_vRPN(a_ByteCode.m_vRPN)
    , m_bEnableOptimizer(true)
{
    Assign(a_ByteCode);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Assignment operator.
 *
 * Implemented in Terms of Assign(const QParserByteCode &a_ByteCode)
 */
QmuParserByteCode& QmuParserByteCode::operator=(const QmuParserByteCode& a_ByteCode)
{
    if (this != &a_ByteCode) {
        Assign(a_ByteCode);
    }
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Copy state of another object to this.
 *
 * @throw nowthrow
 */
void QmuParserByteCode::Assign(const QmuParserByteCode& a_ByteCode)
{
    if (this == &a_ByteCode) {
        return;
    }

    m_iStackPos = a_ByteCode.m_iStackPos;
    m_vRPN = a_ByteCode.m_vRPN;
    m_iMaxStackSize = a_ByteCode.m_iMaxStackSize;
    m_bEnableOptimizer = a_ByteCode.m_bEnableOptimizer;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add a Variable pointer to bytecode.
 * @param a_pVar Pointer to be added.
 * @throw nothrow
 */
void QmuParserByteCode::AddVar(qreal* a_pVar)
{
    ++m_iStackPos;
    m_iMaxStackSize = qMax(m_iMaxStackSize, m_iStackPos);

    // optimization does not apply
    SToken tok;
    tok.Cmd = cmVAR;
    tok.Val.ptr = a_pVar;
    tok.Val.data = 1;
    tok.Val.data2 = 0;
    m_vRPN.push_back(tok);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add a Variable pointer to bytecode.
 *
 * Value entries in byte code consist of:
 * <ul>
 * <li>value array position of the value</li>
 * <li>the operator code according to ParserToken::cmVAL</li>
 * <li>the value stored in #mc_iSizeVal number of bytecode entries.</li>
 * </ul>
 *
 * @param a_fVal Value to be added.
 * @throw nothrow
 */
void QmuParserByteCode::AddVal(qreal a_fVal)
{
    ++m_iStackPos;
    m_iMaxStackSize = qMax(m_iMaxStackSize, m_iStackPos);

    // If optimization does not apply
    SToken tok;
    tok.Cmd = cmVAL;
    tok.Val.ptr = nullptr;
    tok.Val.data = 0;
    tok.Val.data2 = a_fVal;
    m_vRPN.push_back(tok);
}

//---------------------------------------------------------------------------------------------------------------------
void QmuParserByteCode::ConstantFolding(ECmdCode a_Oprt)
{
    int sz = m_vRPN.size();
    qreal &x = m_vRPN[sz - 2].Val.data2, &y = m_vRPN[sz - 1].Val.data2;
    switch (a_Oprt) {
    case cmLAND:
        x = static_cast<int>(x) && static_cast<int>(y);
        m_vRPN.pop_back();
        break;
    case cmLOR:
        x = static_cast<int>(x) || static_cast<int>(y);
        m_vRPN.pop_back();
        break;
    case cmLT:
        x = x < y;
        m_vRPN.pop_back();
        break;
    case cmGT:
        x = x > y;
        m_vRPN.pop_back();
        break;
    case cmLE:
        x = x <= y;
        m_vRPN.pop_back();
        break;
    case cmGE:
        x = x >= y;
        m_vRPN.pop_back();
        break;
    case cmNEQ:
        x = not QmuFuzzyComparePossibleNulls(x, y);
        m_vRPN.pop_back();
        break;
    case cmEQ:
        x = QmuFuzzyComparePossibleNulls(x, y);
        m_vRPN.pop_back();
        break;
    case cmADD:
        x = x + y;
        m_vRPN.pop_back();
        break;
    case cmSUB:
        x = x - y;
        m_vRPN.pop_back();
        break;
    case cmMUL:
        x = x * y;
        m_vRPN.pop_back();
        break;
    case cmDIV:
#if defined(MUP_MATH_EXCEPTIONS)
        if (y == 0) {
            throw qmuParserError(ecDIV_BY_ZERO, "0");
        }
#endif
        x = x / y;
        m_vRPN.pop_back();
        break;
    case cmPOW:
        x = qPow(x, y);
        m_vRPN.pop_back();
        break;
    default: break;
    }   // switch opcode
}

QT_WARNING_PUSH
QT_WARNING_DISABLE_INTEL(1195)
QT_WARNING_DISABLE_MSVC(4826)

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add an operator identifier to bytecode.
 *
 * Operator entries in byte code consist of:
 * <ul>
 * <li>value array position of the result</li>
 * <li>the operator code according to ParserToken::ECmdCode</li>
 * </ul>
 *
 * @sa  ParserToken::ECmdCode
 */
void QmuParserByteCode::AddOp(ECmdCode a_Oprt)
{
    bool bOptimized = false;

    if (m_bEnableOptimizer) {
        int sz = m_vRPN.size();

        // Check for foldable constants like:
        //   cmVAL cmVAL cmADD
        // where cmADD can stand fopr any binary operator applied to
        // two constant values.
        if (sz >= 2 && m_vRPN.at(sz - 2).Cmd == cmVAL && m_vRPN.at(sz - 1).Cmd == cmVAL) {
            ConstantFolding(a_Oprt);
            bOptimized = true;
        } else {
            switch (a_Oprt) {
            case cmPOW:
                // Optimization for ploynomials of low order
                if (m_vRPN.at(sz - 2).Cmd == cmVAR && m_vRPN.at(sz - 1).Cmd == cmVAL)   //-V807
                {
                    if (qFuzzyCompare(m_vRPN.at(sz - 1).Val.data2, 2))   //-V807
                    {
                        m_vRPN[sz - 2].Cmd = cmVARPOW2;
                    } else if (qFuzzyCompare(m_vRPN.at(sz - 1).Val.data2, 3)) {
                        m_vRPN[sz - 2].Cmd = cmVARPOW3;
                    } else if (qFuzzyCompare(m_vRPN.at(sz - 1).Val.data2, 4)) {
                        m_vRPN[sz - 2].Cmd = cmVARPOW4;
                    } else {
                        break;
                    }
                    m_vRPN.pop_back();
                    bOptimized = true;
                }
                break;

            case cmSUB:
            case cmADD:
                // Simple optimization based on pattern recognition for a shitload of different
                // bytecode combinations of addition/subtraction
                if ((m_vRPN.at(sz - 1).Cmd == cmVAR && m_vRPN.at(sz - 2).Cmd == cmVAL)
                    || (m_vRPN.at(sz - 1).Cmd == cmVAL && m_vRPN.at(sz - 2).Cmd == cmVAR)
                    || (m_vRPN.at(sz - 1).Cmd == cmVAL && m_vRPN.at(sz - 2).Cmd == cmVARMUL)
                    || (m_vRPN.at(sz - 1).Cmd == cmVARMUL && m_vRPN.at(sz - 2).Cmd == cmVAL)
                    || (m_vRPN.at(sz - 1).Cmd == cmVAR && m_vRPN.at(sz - 2).Cmd == cmVAR
                        && m_vRPN.at(sz - 2).Val.ptr == m_vRPN.at(sz - 1).Val.ptr)
                    ||   //-V807
                    (m_vRPN.at(sz - 1).Cmd == cmVAR && m_vRPN.at(sz - 2).Cmd == cmVARMUL
                     && m_vRPN.at(sz - 2).Val.ptr == m_vRPN.at(sz - 1).Val.ptr)
                    || (m_vRPN.at(sz - 1).Cmd == cmVARMUL && m_vRPN.at(sz - 2).Cmd == cmVAR
                        && m_vRPN.at(sz - 2).Val.ptr == m_vRPN.at(sz - 1).Val.ptr)
                    || (m_vRPN.at(sz - 1).Cmd == cmVARMUL && m_vRPN.at(sz - 2).Cmd == cmVARMUL
                        && m_vRPN.at(sz - 2).Val.ptr == m_vRPN.at(sz - 1).Val.ptr)) {
                    assert(
                        (m_vRPN.at(sz - 2).Val.ptr == nullptr
                         && m_vRPN.at(sz - 1).Val.ptr != nullptr)
                        || (m_vRPN.at(sz - 2).Val.ptr != nullptr
                            && m_vRPN.at(sz - 1).Val.ptr == nullptr)
                        || (m_vRPN.at(sz - 2).Val.ptr == m_vRPN.at(sz - 1).Val.ptr));

                    m_vRPN[sz - 2].Cmd = cmVARMUL;
                    m_vRPN[sz - 2].Val.ptr = reinterpret_cast<qreal*>(   //-V807
                        reinterpret_cast<qlonglong>(m_vRPN.at(sz - 2).Val.ptr)
                        | reinterpret_cast<qlonglong>(m_vRPN.at(sz - 1).Val.ptr));   // variable
                    m_vRPN[sz - 2].Val.data2 +=
                        ((a_Oprt == cmSUB) ? -1 : 1) * m_vRPN.at(sz - 1).Val.data2;   // offset
                    m_vRPN[sz - 2].Val.data += ((a_Oprt == cmSUB) ? -1 : 1)
                                             * m_vRPN.at(sz - 1).Val.data;   // multiplikatior
                    m_vRPN.pop_back();
                    bOptimized = true;
                }
                break;
            case cmMUL:
                if ((m_vRPN.at(sz - 1).Cmd == cmVAR && m_vRPN.at(sz - 2).Cmd == cmVAL)
                    || (m_vRPN.at(sz - 1).Cmd == cmVAL && m_vRPN.at(sz - 2).Cmd == cmVAR)) {
                    m_vRPN[sz - 2].Cmd = cmVARMUL;
                    m_vRPN[sz - 2].Val.ptr = reinterpret_cast<qreal*>(
                        reinterpret_cast<qlonglong>(m_vRPN.at(sz - 2).Val.ptr)
                        | reinterpret_cast<qlonglong>(m_vRPN.at(sz - 1).Val.ptr));
                    m_vRPN[sz - 2].Val.data =
                        m_vRPN.at(sz - 2).Val.data2 + m_vRPN.at(sz - 1).Val.data2;
                    m_vRPN[sz - 2].Val.data2 = 0;
                    m_vRPN.pop_back();
                    bOptimized = true;
                } else if (
                    (m_vRPN.at(sz - 1).Cmd == cmVAL && m_vRPN.at(sz - 2).Cmd == cmVARMUL)
                    || (m_vRPN.at(sz - 1).Cmd == cmVARMUL && m_vRPN.at(sz - 2).Cmd == cmVAL)) {
                    // Optimization: 2*(3*b+1) or (3*b+1)*2 -> 6*b+2
                    m_vRPN[sz - 2].Cmd = cmVARMUL;
                    m_vRPN[sz - 2].Val.ptr = reinterpret_cast<qreal*>(
                        reinterpret_cast<qlonglong>(m_vRPN.at(sz - 2).Val.ptr)
                        | reinterpret_cast<qlonglong>(m_vRPN.at(sz - 1).Val.ptr));
                    if (m_vRPN.at(sz - 1).Cmd == cmVAL) {
                        m_vRPN[sz - 2].Val.data *= m_vRPN.at(sz - 1).Val.data2;
                        m_vRPN[sz - 2].Val.data2 *= m_vRPN.at(sz - 1).Val.data2;
                    } else {
                        m_vRPN[sz - 2].Val.data =
                            m_vRPN.at(sz - 1).Val.data * m_vRPN.at(sz - 2).Val.data2;
                        m_vRPN[sz - 2].Val.data2 =
                            m_vRPN.at(sz - 1).Val.data2 * m_vRPN.at(sz - 2).Val.data2;
                    }
                    m_vRPN.pop_back();
                    bOptimized = true;
                } else if (
                    m_vRPN.at(sz - 1).Cmd == cmVAR && m_vRPN.at(sz - 2).Cmd == cmVAR
                    && m_vRPN.at(sz - 1).Val.ptr == m_vRPN.at(sz - 2).Val.ptr) {
                    // Optimization: a*a -> a^2
                    m_vRPN[sz - 2].Cmd = cmVARPOW2;
                    m_vRPN.pop_back();
                    bOptimized = true;
                }
                break;
            case cmDIV:
                if (m_vRPN.at(sz - 1).Cmd == cmVAL && m_vRPN.at(sz - 2).Cmd == cmVARMUL
                    && not qFuzzyIsNull(m_vRPN.at(sz - 1).Val.data2)) {
                    // Optimization: 4*a/2 -> 2*a
                    m_vRPN[sz - 2].Val.data /= m_vRPN.at(sz - 1).Val.data2;
                    m_vRPN[sz - 2].Val.data2 /= m_vRPN.at(sz - 1).Val.data2;
                    m_vRPN.pop_back();
                    bOptimized = true;
                }
                break;
            default: break;

            }   // switch a_Oprt
        }
    }

    // If optimization can't be applied just write the value
    if (bOptimized == false) {
        --m_iStackPos;
        SToken tok;
        tok.Cmd = a_Oprt;
        m_vRPN.push_back(tok);
    }
}

QT_WARNING_POP

//---------------------------------------------------------------------------------------------------------------------
void QmuParserByteCode::AddIfElse(ECmdCode a_Oprt)
{
    SToken tok;
    tok.Cmd = a_Oprt;
    m_vRPN.push_back(tok);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add an assignement operator
 *
 * Operator entries in byte code consist of:
 * <ul>
 * <li>cmASSIGN code</li>
 * <li>the pointer of the destination variable</li>
 * </ul>
 *
 * @sa  ParserToken::ECmdCode
 */
void QmuParserByteCode::AddAssignOp(qreal* a_pVar)
{
    --m_iStackPos;

    SToken tok;
    tok.Cmd = cmASSIGN;
    tok.Oprt.ptr = a_pVar;
    m_vRPN.push_back(tok);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add function to bytecode.
 *
 * @param a_iArgc Number of arguments, negative numbers indicate multiarg functions.
 * @param a_pFun Pointer to function callback.
 */
void QmuParserByteCode::AddFun(generic_fun_type a_pFun, int a_iArgc)
{
    if (a_iArgc >= 0) {
        m_iStackPos = m_iStackPos - static_cast<quint32>(a_iArgc) + 1;
    } else {
        // function with unlimited number of arguments
        m_iStackPos = static_cast<quint32>(static_cast<int>(m_iStackPos) + a_iArgc + 1);
    }
    m_iMaxStackSize = qMax(m_iMaxStackSize, m_iStackPos);

    SToken tok;
    tok.Cmd = cmFUNC;
    tok.Fun.argc = a_iArgc;
    tok.Fun.ptr = a_pFun;
    m_vRPN.push_back(tok);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add a bulk function to bytecode.
 *
 * @param a_iArgc Number of arguments, negative numbers indicate multiarg functions.
 * @param a_pFun Pointer to function callback.
 */
void QmuParserByteCode::AddBulkFun(generic_fun_type a_pFun, int a_iArgc)
{
    m_iStackPos = static_cast<quint32>(static_cast<int>(m_iStackPos) - a_iArgc + 1);
    m_iMaxStackSize = qMax(m_iMaxStackSize, m_iStackPos);

    SToken tok;
    tok.Cmd = cmFUNC_BULK;
    tok.Fun.argc = a_iArgc;
    tok.Fun.ptr = a_pFun;
    m_vRPN.push_back(tok);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add Strung function entry to the parser bytecode.
 * @throw nothrow
 *
 * A string function entry consists of the stack position of the return value, followed by a
 * cmSTRFUNC code, the function pointer and an index into the string buffer maintained by the
 * parser.
 */
void QmuParserByteCode::AddStrFun(generic_fun_type a_pFun, int a_iArgc, int a_iIdx)
{
    m_iStackPos = static_cast<quint32>(static_cast<int>(m_iStackPos) - a_iArgc + 1);

    SToken tok;
    tok.Cmd = cmFUNC_STR;
    tok.Fun.argc = a_iArgc;
    tok.Fun.idx = a_iIdx;
    tok.Fun.ptr = a_pFun;
    m_vRPN.push_back(tok);

    m_iMaxStackSize = qMax(m_iMaxStackSize, m_iStackPos);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Add end marker to bytecode.
 *
 * @throw nothrow
 */
void QmuParserByteCode::Finalize()
{
    SToken tok;
    tok.Cmd = cmEND;
    m_vRPN.push_back(tok);
    rpn_type(m_vRPN).swap(m_vRPN);   // shrink bytecode vector to fit

    // Determine the if-then-else jump offsets
    QStack<int> stIf, stElse;
    int idx;
    for (int i = 0; i < m_vRPN.size(); ++i) {
        switch (m_vRPN.at(i).Cmd) {
        case cmIF: stIf.push(i); break;
        case cmELSE:
            stElse.push(i);
            idx = stIf.pop();
            m_vRPN[idx].Oprt.offset = i - idx;
            break;
        case cmENDIF:
            idx = stElse.pop();
            m_vRPN[idx].Oprt.offset = i - idx;
            break;
        default: break;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
const SToken* QmuParserByteCode::GetBase() const
{
    if (m_vRPN.size() == 0) {
        throw QmuParserError(ecINTERNAL_ERROR);
    } else {
        return &m_vRPN.at(0);
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Delete the bytecode.
 *
 * @throw nothrow
 *
 * The name of this function is a violation of my own coding guidelines but this way it's more in
 * line with the STL functions thus more intuitive.
 */
void QmuParserByteCode::clear()
{
    m_vRPN.clear();
    m_iStackPos = 0;
    m_iMaxStackSize = 0;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief Dump bytecode (for debugging only!).
 */
void QmuParserByteCode::AsciiDump()
{
    if (m_vRPN.size() == false) {
        qWarning() << "No bytecode available\n";
        return;
    }

    qInfo() << "Number of RPN tokens:" << m_vRPN.size() << "\n";
    for (int i = 0; i < m_vRPN.size() && m_vRPN.at(i).Cmd != cmEND; ++i) {
        qInfo() << i << " : \t";
        switch (m_vRPN.at(i).Cmd) {
        case cmVAL:
            qInfo() << "VAL \t"
                    << "[" << m_vRPN.at(i).Val.data2 << "]\n";
            break;
        case cmVAR:
            qInfo() << "VAR \t"
                    << "[ADDR: 0x" << QString::number(*m_vRPN.at(i).Val.ptr, 'f', 16) << "]\n";
            break;
        case cmVARPOW2:
            qInfo() << "VARPOW2 \t"
                    << "[ADDR: 0x" << QString::number(*m_vRPN.at(i).Val.ptr, 'f', 16) << "]\n";
            break;
        case cmVARPOW3:
            qInfo() << "VARPOW3 \t"
                    << "[ADDR: 0x" << QString::number(*m_vRPN.at(i).Val.ptr, 'f', 16) << "]\n";
            break;
        case cmVARPOW4:
            qInfo() << "VARPOW4 \t"
                    << "[ADDR: 0x" << QString::number(*m_vRPN.at(i).Val.ptr, 'f', 16) << "]\n";
            break;
        case cmVARMUL:
            qInfo() << "VARMUL \t"
                    << "[ADDR: 0x" << QString::number(*m_vRPN.at(i).Val.ptr, 'f', 16) << "]"
                    << " * ["   //-V807
                    << m_vRPN.at(i).Val.data << "]"
                    << " + [" << m_vRPN.at(i).Val.data2 << "]\n";
            break;
        case cmFUNC:
            qInfo() << "CALL\t"
                    << "[ARG:" << m_vRPN.at(i).Fun.argc << "]"
                    << "[ADDR: 0x" << m_vRPN.at(i).Fun.ptr << "]"
                    << "\n";
            break;
        case cmFUNC_STR:
            qInfo() << "CALL STRFUNC\t"
                    << "[ARG:" << m_vRPN.at(i).Fun.argc << "]"
                    << "[IDX:" << m_vRPN.at(i).Fun.idx   //-V807
                    << "]"
                    << "[ADDR: 0x" << m_vRPN.at(i).Fun.ptr << "]\n";
            break;
        case cmLT: qInfo() << "LT\n"; break;
        case cmGT: qInfo() << "GT\n"; break;
        case cmLE: qInfo() << "LE\n"; break;
        case cmGE: qInfo() << "GE\n"; break;
        case cmEQ: qInfo() << "EQ\n"; break;
        case cmNEQ: qInfo() << "NEQ\n"; break;
        case cmADD: qInfo() << "ADD\n"; break;
        case cmLAND: qInfo() << "&&\n"; break;
        case cmLOR: qInfo() << "||\n"; break;
        case cmSUB: qInfo() << "SUB\n"; break;
        case cmMUL: qInfo() << "MUL\n"; break;
        case cmDIV: qInfo() << "DIV\n"; break;
        case cmPOW: qInfo() << "POW\n"; break;
        case cmIF:
            qInfo() << "IF\t"
                    << "[OFFSET:" << m_vRPN.at(i).Oprt.offset << "]\n";
            break;
        case cmELSE:
            qInfo() << "ELSE\t"
                    << "[OFFSET:" << m_vRPN.at(i).Oprt.offset << "]\n";
            break;
        case cmENDIF: qInfo() << "ENDIF\n"; break;
        case cmASSIGN:
            qInfo() << "ASSIGN\t"
                    << "[ADDR: 0x" << QString::number(*m_vRPN.at(i).Oprt.ptr, 'f', 16) << "]\n";
            break;
        case cmBO:
        case cmBC:
        case cmARG_SEP:
        case cmPOW2:
        case cmFUNC_BULK:
        case cmSTRING:
        case cmOPRT_BIN:
        case cmOPRT_POSTFIX:
        case cmOPRT_INFIX:
        case cmEND:
        case cmUNKNOWN:
        default: qWarning() << "(unknown code: " << m_vRPN.at(i).Cmd << ")\n"; break;
        }   // switch cmdCode
    }       // while bytecode

    qInfo() << "END";
}
}   // namespace qmu
