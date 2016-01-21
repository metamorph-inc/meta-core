#ifndef EVALUATE_FORMULA_H
#define EVALUATE_FORMULA_H

#include "UdmConsole.h"

#include "string_utils.h"
#include "muParser.h"



/** \file
    \brief Definition of function that calls muParser parser for formula evaluation.		    
*/

/** \brief Creates an expression and defines variables in muParser
    \param [in] parameters A map of parameters with parameter name as the key
	\param [in] parser Reference to the muParser
	\param [in] delimiter Delimiter used to seperate parameters
    \return void
*/
/*
std::wstring CreateExpression(multimap<std::wstring, double*> &parameters, mu::Parser &parser, std::wstring delimiter = ",")
{
	std::wstring expression;
	for(multimap<std::wstring, double*>::iterator i = parameters.begin();i != parameters.end();++i)
	{
		if (i != parameters.begin())
			expression += delimiter;

		expression += i->first;  

		parser.DefineVar(i->first, i->second);
	}

	return expression;
}
*/

/** \brief Creates an expression that directly uses parameter values, this eliminates need to call DefineVar in muParser
    \param [in] parameters A map of parameters with parameter name as the key
	\param [in] parser Reference to the muParser
	\param [in] delimiter Delimiter used to seperate parameters
    \return void
*/
std::wstring CreateExpression2(multimap<std::wstring, double> &parameters, mu::Parser &parser, std::wstring delimiter = L",")
{
	std::wstring expression;
	for(multimap<std::wstring, double>::iterator i = parameters.begin();i != parameters.end();++i)
	{
		if (i != parameters.begin())
			expression += delimiter;

		std::wstring tmp;
		to_string(tmp, i->second);
		expression += tmp;  
	}

	return expression;
}

/** \brief Checks tabs and newlines in expression and remove them if found
    \param [in] expression Expression
    \return reformatted expression
*/
std::wstring CheckExpression(std::wstring expression)
{
	std::wstring cleanExpression;

	for (std::wstring::size_type i = 0; i < expression.size(); i++)
	{
		if (expression[i] != '\n' && expression[i] != '\t')
			cleanExpression += expression[i];
	}
	return cleanExpression;
}

/** \brief Manual addition calculation w/o using muparser
    \param [in] parameters Parameters to be added together
    \return calculated result
*/
double EvaluateAddition(multimap<std::wstring, double> &parameters)
{
	double result = 0;
	for(multimap<std::wstring, double>::iterator i = parameters.begin(); i != parameters.end();++i)
	{
		double val = (i->second);
		result += val;
	}

	return result;
}

/** \brief Manual multiplication calculation w/o using muparser
    \param [in] parameters Parameters to be multiplied together
    \return calculated result
*/
double EvaluateMultiplication(multimap<std::wstring, double> &parameters)
{
	double result = 0;

	for(multimap<std::wstring, double>::iterator i = parameters.begin();i != parameters.end();++i)
	{
		if (i == parameters.begin())
			result = i->second;
		else
			result *= i->second;  
	}
	
	return result;
}

/** \brief Manual geometric mean calculation w/o using muparser
    \param [in] parameters Parameters to be added together
    \return calculated result
*/
double EvaluateGeometricMean(multimap<std::wstring, double> &parameters)
{
	// If any of the parameters are negative, give an erorr and return a negative result
	for(multimap<std::wstring, double>::iterator i = parameters.begin(); i != parameters.end(); ++i)
	{
		if (i->second < 0)
		{
			GMEConsole::Console::writeLine("Error: cannot evaluate the geometric mean of a negative number.", MSG_ERROR);
			return -1;
		}
	}

	double result = 0;
	result = EvaluateMultiplication(parameters);
	
	if(!parameters.empty())
	{
		double x = double(1)/parameters.size();
		result = pow(result, x);
	}
	return result;
}

/** \brief Manual maximum finding w/o using muparser
    \param [in] parameters Parameters to be evaluated
    \return maximum value
*/
double EvaluateMaximum(multimap<std::wstring, double> &parameters)
{
	double result = 0;

	if(parameters.empty()) 
		return result;

	result = ((parameters.begin())->second);
	for(multimap<std::wstring, double>::iterator i = parameters.begin();i != parameters.end(); i++)
		if((i->second) > result) 
			result = (i->second);

	return result;
}

/** \brief Manual minimum finding w/o using muparser
    \param [in] parameters Parameters to be evaluated
    \return minimum value
*/
double EvaluateMinimum(multimap<std::wstring, double> &parameters)
{
	double result = 0;
	if(parameters.empty()) 
		return result;

	result = ((parameters.begin())->second);
	for(multimap<std::wstring, double>::iterator i=parameters.begin();i!=parameters.end();++i)
		if((i->second) < result) 
			result = (i->second);

	return result;
}

/** \brief Evaluates Custom Formulas
    \param [in] parameters Used in calculation
    \param [in] expression Custom formula expression to be evaluated
    \return calculated result
*/
double EvaluateCustomFormula(multimap<std::wstring, double> &parameters, std::wstring expression)
{
	double result = 0;
	try
	{
		mu::Parser parser;
		parser.ResetLocale();

		for(multimap<std::wstring, double>::iterator i = parameters.begin(); i != parameters.end(); i++)
			parser.DefineVar((*i).first, &(*i).second);
			//parser.DefineVar(i->first, i->second);

		parser.SetExpr(CheckExpression(expression));
		result = parser.Eval();

		if (result == std::numeric_limits<double>::infinity())
		{
			auto exc = new std::exception("Expression resulted in +INF or -INF");
			throw exc;
		}
	}
	catch (mu::Parser::exception_type &e)
	{
		GMEConsole::Console::writeLine(L"muParser Exception [ expression = " + expression + L" ] Error: " + e.GetMsg(), MSG_ERROR);
	}
	catch (std::exception &e)
	{
		GMEConsole::Console::writeLine("Exception: " + (std::string)e.what(), MSG_ERROR);
	}
	catch (...)
	{
		GMEConsole::Console::writeLine("Exception! Not sure what though!", MSG_ERROR);
	}

	return result;
}

/** \brief Evaluates Simple Formulas using muParser when possible
    \param [in] parameters Used in calculation
    \param [in] operation Type of operation to be performed
    \return calculated result
*/
double EvaluateSimpleFormula(multimap<std::wstring, double> &parameters, std::wstring operation)
{		
	
	double result = 0;
	try 
	{
		mu::Parser parser;

		if (operation == L"Addition")
		{	
			std::wstring expression = L"sum(" + CreateExpression2(parameters, parser) + L")";
			parser.SetExpr(expression);
			result = parser.Eval();		
		}
		else if (operation == L"Multiplication")
		{
			//result = EvaluateMultiplication(parameters);
			parser.SetExpr(CreateExpression2(parameters, parser, L"*"));
			result = parser.Eval(); 
		}
		else if (operation == L"ArithmeticMean")
		{
			std::wstring expression = L"avg(" + CreateExpression2(parameters, parser) + L")";
			parser.SetExpr(expression);
			result = parser.Eval();	
		}
		else if (operation == L"GeometricMean")
			result = EvaluateGeometricMean(parameters);
		else if (operation == L"Maximum")
		{
			std::wstring expression = L"max(" + CreateExpression2(parameters, parser) + L")";
			parser.SetExpr(expression);
			result = parser.Eval();	
		}
		else if (operation == L"Minimum")
		{
			std::wstring expression = L"min(" + CreateExpression2(parameters, parser) + L")";
			parser.SetExpr(expression);
			result = parser.Eval();	
		}
	}
	catch (mu::Parser::exception_type &e)
	{
		GMEConsole::Console::writeLine(L"muParser Exception: " + e.GetMsg(), MSG_ERROR);
	}
	catch (std::exception &e)
	{
		GMEConsole::Console::writeLine("Exception: " + (std::string)e.what(), MSG_ERROR);
	}
	return result;
}

#endif