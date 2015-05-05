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
std::string CreateExpression(multimap<std::string, double*> &parameters, mu::Parser &parser, std::string delimiter = ",")
{
	std::string expression;
	for(multimap<std::string, double*>::iterator i = parameters.begin();i != parameters.end();++i)
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
std::string CreateExpression2(multimap<std::string, double> &parameters, mu::Parser &parser, std::string delimiter = ",")
{
	std::string expression;
	for(multimap<std::string, double>::iterator i = parameters.begin();i != parameters.end();++i)
	{
		if (i != parameters.begin())
			expression += delimiter;

		std::string tmp;
		to_string(tmp, i->second);
		expression += tmp;  
	}

	return expression;
}

/** \brief Checks tabs and newlines in expression and remove them if found
    \param [in] expression Expression
    \return reformatted expression
*/
std::string CheckExpression(std::string expression)
{
	std::string cleanExpression;

	for (std::string::size_type i = 0; i < expression.size(); i++)
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
double EvaluateAddition(multimap<std::string, double> &parameters)
{
	double result = 0;
	for(multimap<std::string, double>::iterator i = parameters.begin(); i != parameters.end();++i)
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
double EvaluateMultiplication(multimap<std::string, double> &parameters)
{
	double result = 0;

	for(multimap<std::string, double>::iterator i = parameters.begin();i != parameters.end();++i)
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
double EvaluateGeometricMean(multimap<std::string, double> &parameters)
{
	// If any of the parameters are negative, give an erorr and return a negative result
	for(multimap<std::string, double>::iterator i = parameters.begin(); i != parameters.end(); ++i)
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
double EvaluateMaximum(multimap<std::string, double> &parameters)
{
	double result = 0;

	if(parameters.empty()) 
		return result;

	result = ((parameters.begin())->second);
	for(multimap<std::string, double>::iterator i = parameters.begin();i != parameters.end(); i++)
		if((i->second) > result) 
			result = (i->second);

	return result;
}

/** \brief Manual minimum finding w/o using muparser
    \param [in] parameters Parameters to be evaluated
    \return minimum value
*/
double EvaluateMinimum(multimap<std::string, double> &parameters)
{
	double result = 0;
	if(parameters.empty()) 
		return result;

	result = ((parameters.begin())->second);
	for(multimap<std::string, double>::iterator i=parameters.begin();i!=parameters.end();++i)
		if((i->second) < result) 
			result = (i->second);

	return result;
}

/** \brief Evaluates Custom Formulas
    \param [in] parameters Used in calculation
    \param [in] expression Custom formula expression to be evaluated
    \return calculated result
*/
double EvaluateCustomFormula(multimap<std::string, double> &parameters, std::string expression)
{
	double result = 0;
	try
	{
		mu::Parser parser;
		parser.ResetLocale();

		for(multimap<std::string, double>::iterator i = parameters.begin(); i != parameters.end(); i++)
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
		GMEConsole::Console::writeLine("muParser Exception [ expression = " + expression + " ] Error: " + e.GetMsg(), MSG_ERROR);
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
double EvaluateSimpleFormula(multimap<std::string, double> &parameters, std::string operation)
{		
	
	double result = 0;
	try 
	{
		mu::Parser parser;

		if (operation == "Addition")
		{	
			std::string expression = "sum(" + CreateExpression2(parameters, parser) + ")";
			parser.SetExpr(expression);
			result = parser.Eval();		
		}
		else if (operation == "Multiplication")
		{
			//result = EvaluateMultiplication(parameters);
			parser.SetExpr(CreateExpression2(parameters, parser, "*"));
			result = parser.Eval(); 
		}
		else if (operation == "ArithmeticMean")
		{
			std::string expression = "avg(" + CreateExpression2(parameters, parser) + ")";
			parser.SetExpr(expression);
			result = parser.Eval();	
		}
		else if (operation == "GeometricMean")
			result = EvaluateGeometricMean(parameters);
		else if (operation == "Maximum")
		{
			std::string expression = "max(" + CreateExpression2(parameters, parser) + ")";
			parser.SetExpr(expression);
			result = parser.Eval();	
		}
		else if (operation == "Minimum")
		{
			std::string expression = "min(" + CreateExpression2(parameters, parser) + ")";
			parser.SetExpr(expression);
			result = parser.Eval();	
		}
	}
	catch (mu::Parser::exception_type &e)
	{
		GMEConsole::Console::writeLine("muParser Exception: " + e.GetMsg(), MSG_ERROR);
	}
	catch (std::exception &e)
	{
		GMEConsole::Console::writeLine("Exception: " + (std::string)e.what(), MSG_ERROR);
	}
	return result;
}

#endif