namespace DesignConsistencyChecker.DesignRule
{
    public class GenericRuleFeedback<T> : RuleFeedbackBase
    {
        public T AdditionalInfo { get; set; }
    }

    public class GenericRuleFeedback : RuleFeedbackBase
    {

    }
}