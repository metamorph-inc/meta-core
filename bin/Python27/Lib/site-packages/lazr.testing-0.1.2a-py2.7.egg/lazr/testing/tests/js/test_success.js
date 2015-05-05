SuccessTestCase = TestCase("SuccessTestCase");

SuccessTestCase.prototype.testAssertEquals = function() {
    assertEquals("Hello", "Hello");
};

SuccessTestCase.prototype.testAssertTrue = function() {
    assertTrue("How can that possibly be false?", true);
};

SuccessInlineTestCase = TestCase("SuccessInlineTestCase", {
    testAssertFalse: function() {
        assertFalse("Not false? Are you kidding me?", false);
    },
    testAssertNull: function() {
        assertNull("I was pretty sure it was null.", null);
    }
});
