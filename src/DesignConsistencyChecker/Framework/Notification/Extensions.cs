using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq.Expressions;

namespace DesignConsistencyChecker.Framework.Notification
{
    internal static class Extensions
    {

        public static bool ChangeAndNotify<T>(this PropertyChangedEventHandler handler, ref T field, T value,
                                              Expression<Func<T>> memberExpression)
        {
            if (memberExpression == null)
            {
                throw new ArgumentNullException("memberExpression");
            }
            var body = memberExpression.Body as MemberExpression;
            if (body == null)
            {
                throw new ArgumentException("Lambda must return a property.");
            }
            if (EqualityComparer<T>.Default.Equals(field, value))
            {
                return false;
            }

            var vmExpression = body.Expression as ConstantExpression;
            if (vmExpression != null)
            {
                LambdaExpression lambda = Expression.Lambda(vmExpression);
                Delegate vmFunc = lambda.Compile();
                object sender = vmFunc.DynamicInvoke();

                if (handler != null)
                {
                    handler(sender, new PropertyChangedEventArgs(body.Member.Name));
                }
            }

            field = value;
            return true;
        }

        public static void Raise(this PropertyChangedEventHandler handler, object sender,
                                 Expression<Func<object>> expression)
        {
            if (handler == null) return;
            if (expression.NodeType != ExpressionType.Lambda)
            {

                throw new ArgumentException("Value must be a lamda expression", "expression");

            }

            var unary = expression.Body as UnaryExpression;
            MemberExpression body;

            if (unary != null)
                body = unary.Operand as MemberExpression;
            else
                body = expression.Body as MemberExpression;

            if (body == null)
            {
                throw new ArgumentException("'x' should be a member expression");
            }

            var propertyName = body.Member.Name;

            handler(sender, new PropertyChangedEventArgs(propertyName));
        }
    }
}