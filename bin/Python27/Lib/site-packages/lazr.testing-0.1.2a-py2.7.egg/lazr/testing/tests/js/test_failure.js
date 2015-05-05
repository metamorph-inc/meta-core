FailureTestCase = TestCase("FailureTestCase");

FailureTestCase.prototype.testAssertEquals = function() {
    assertEquals("Hello", "World!");
};

FailureTestCase.prototype.testAssertTrue = function() {
    assertTrue("How can that possibly be false?", false);
};

FailureInlineTestCase = TestCase("FailureInlineTestCase", {
    testAssertFalse: function() {
        assertFalse("Not false? Are you kidding me?", true);
    },
    testAssertNull: function() {
        assertNull("I was pretty sure it was null.", {});
    }
});
