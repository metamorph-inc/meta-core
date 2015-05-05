ErrorTestCase = TestCase("ErrorTestCase");

ErrorTestCase.prototype.testAssertEquals = function() {
    var x = "Hello";
    assertEquals("Hello", y);
};

ErrorTestCase.prototype.testAssertTrue = function() {
    var x = true;
    assertTrue("How can that possibly be false?", y);
};

ErrorInlineTestCase = TestCase("ErrorInlineTestCase", {
    testAssertFalse: function() {
        var x = false;
        assertFalse("Not false? Are you kidding me?", y);
    },
    testAssertNull: function() {
        var x = null;
        assertNull("I was pretty sure it was null.", y);
    },
    testThrowAFit: function() {
        throw("A Fit");
    },
    testThrowAnError: function() {
        var error = new Error();
        error.name = "An Error";
        error.message = "Some Messy Message";
        throw(error);
    }

});
