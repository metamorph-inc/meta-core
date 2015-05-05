// Utility functions

/**
 * findStringInArray
 */
function findStringInArray(array,string) {
    for (var j=0; j<array.length; j++) {
        if (array[j].match(string)) return j
    }
    return -1;
}

/*
 *
 * Loading CSS document and instert it as a link
 *
 */
function getCSS(url) {
    $("head").append("<link>");
    css = $("head").children(":last");
    css.attr({
        rel:  "stylesheet",
        type: "text/css",
        href: url
    });
}

/*
 *
 * Disabling selection on element
 *
 */
// jQuery.fn.extend({
//         disableSelection : function() {
//                 this.each(function() {
//                         this.onselectstart = function() { return false; };
//                         this.unselectable = "on";
//                         jQuery(this).css('-moz-user-select', 'none');
//                 });
//         }
// });


/*
 *
 * Getting textwidth
 *
 */
$.fn.textWidth = function(){
    var html_org = $(this).html();
    var html_calc = '<span>' + html_org + '</span>'
    $(this).html(html_calc);
    var width = $(this).find('span:first').width();
    $(this).html(html_org);
    return width;
};

/*
 *
 * Convert to scientific notation
 *
 */
sciNotation = function(temp){
	var exp = '';
    if(temp.toString().indexOf("e")!= -1){
		var start = temp.toString().indexOf("e");
		exp = temp.toString().substring(start, temp.length);
		var temp = temp*Math.pow(10,parseInt(exp.substring(2,exp.length)));
		var expValue = exp.substring(1,exp.length);
		exp = "e^"+exp.substring(1,exp.length)+"";
	}
	else{
		exp = "e^0";
		expValue = 0;
	}
	
	return {
		number:temp,
		exp:exp,
		expValue:+expValue
	};
};

/**
 *
 * GUID generation
 *
 */
function S4() {
    return (((1+Math.random())*0x10000)|0).toString(16).substring(1);
}
function guid() {
    return (S4()+S4()+"-"+S4()+"-"+S4()+"-"+S4()+"-"+S4()+S4()+S4());
}

/**
 *
 *	Checks if a variable is set
 *
 */
function isSet(variable_, variables_) {
	if(typeof variables_ !== 'undefined'){
		for(var i_ = 0; i_ < Object.keys(variables_).length; i_++){
			this[Object.keys(variables_)[i_]] = variables_[Object.keys(variables_)[i_]];
		}
	}
    return eval("try{" + variable_ + "}catch(e){}") !== undefined;
}

/**
 *
 *	Traces a msg on the console if available
 *
 */
function trace( msg, mode ) {

    if (isSet(window.DEBUG) && window.DEBUG==true) {
        if (isSet(window["console"])) {

            switch (mode) {
                case "error":

                    if (isSet(console.error)) {
                        var today=new Date();

                        var h=today.getHours();
                        var m=today.getMinutes();
                        var s=today.getSeconds();
                        var ms=today.getMilliseconds();

                        console.error("["+h+":"+m+":"+s+"."+ms+"] "+msg);

                        break;
                    }

                default:

                    if (isSet(console.log)) {
                        var today=new Date();

                        var h=today.getHours();
                        var m=today.getMinutes();
                        var s=today.getSeconds();
                        var ms=today.getMilliseconds();

                        console.log("["+h+":"+m+":"+s+"."+ms+"] "+msg);
                    }
            }
        }
    }
}

/**
 *
 * Loads an XML document a processes it with a processor function
 *
 */
function ProcessableXML() {

}

ProcessableXML.prototype = {

    contentLoaded: false,
    contentProcessed: false,
    contentXML: null,
    url: null,
    context: this,

    processor: null,
    postLoading: null,

    _processLoadedContent: function(xml) {

        this.contentXML = xml;
        trace("Content loaded from ["+this.url+"]");
        this.contentLoaded = true;

        if (jQuery.isFunction(this.processor)) {
            this.contentProcessed = this.processor.call(this.context, this.contentXML);
        } else {
            this.contentProcessed = true;
        }
        trace("Content from ["+this.url+"] process result is "+this.contentProcessed);

        if (jQuery.isFunction(this.postLoading)) {
            this.postLoading.call(this.context, this.contentLoaded, this.contentProcessed);
        }
    },

    _loadError: function(XMLHttpRequest, textStatus, errorThrown) {
        trace("Content from ["+this.url+"] could not be loaded. Error msg: "+textStatus+" "+errorThrown, "error");
        if (jQuery.isFunction(this.postLoading)) {
            this.postLoading.call(this.context, this.contentLoaded, this.contentProcessed);
        }
    },

    loadContent: function (location) {

        this.url = location;
        trace("Loading content from ["+this.url+"]");
        this.contentLoaded = false;
        this.contentProcessed = false;

        $.ajax({
            type: 'GET',
            url: this.url,
            dataType: ($.browser.msie) ? "text" : "xml",
            success: this._processLoadedContent,
            error: this._loadError,
            context: this,
            global: false
        });

    },

    setContent: function (xml) {

        this.url = "INTERNALLY_SPECIFIED";


        if (xml != null) {
            this.contentXML = xml;
            trace("Processable XML content set");
            contentLoaded = true;
            if (jQuery.isFunction(this.processor)) {
                this.contentProcessed = this.processor(this.contentXML);
            } else {
                this.contentProcessed = true;
            }
            trace("Content of ["+this.url+"] processing result is "+this.contentProcessed);

            if (jQuery.isFunction(this.postLoading)) {
                this.postLoading.call(this.context, this.contentLoaded, this.contentProcessed);
            }
        } else {
            // this is just for clearing and reseting the object

            delete contentXML;

            contentLoaded = false;
            contentProcessed = false;

        }

    }
}


// Canvas drawing extension
if (!!document.createElement('canvas').getContext) {
    $.extend(CanvasRenderingContext2D.prototype, {

        ellipse: function (aX, aY, r1, r2, fillIt) {
            aX = aX - r1;
            aY = aY - r2;

            var aWidth = r1*2;
            var aHeight = r2*2;

            var hB = (aWidth / 2) * .5522848,
                vB = (aHeight / 2) * .5522848,
                eX = aX + aWidth,
                eY = aY + aHeight,
                mX = aX + aWidth / 2,
                mY = aY + aHeight / 2;
            this.beginPath();
            this.moveTo(aX, mY);
            this.bezierCurveTo(aX, mY - vB, mX - hB, aY, mX, aY);
            this.bezierCurveTo(mX + hB, aY, eX, mY - vB, eX, mY);
            this.bezierCurveTo(eX, mY + vB, mX + hB, eY, mX, eY);
            this.bezierCurveTo(mX - hB, eY, aX, mY + vB, aX, mY);
            this.closePath();
            if (fillIt) this.fill();
            this.stroke();
        },

        circle: function(aX, aY, aDiameter, fillIt) {
            this.ellipse(aX, aY, aDiameter, aDiameter, fillIt)
        }
    });
}

/*
 *
 * EventDispatcher
 *
 */
function EventDispatcher(){
    this._eventList = {};
};
EventDispatcher.prototype = {
    _eventList: null,
    _getEvent: function(eventName, create){
        // Check if Array of Event Handlers has been created
        if (!this._eventList[eventName]){

            // Check if the calling method wants to create the Array
            // if not created. This reduces unneeded memory usage.
            if (!create){
                return null;
            }

            // Create the Array of Event Handlers
            this._eventList[eventName] = [];
            // new Array
        }

        // return the Array of Event Handlers already added
        return this._eventList[eventName];
    },
    addEventListener: function(eventName, handler){
        // Get the Array of Event Handlers
        var evt = this._getEvent(eventName, true);

        // Add the new Event Handler to the Array
        evt.push(handler);
    },
    removeEventListener: function(eventName, handler){
        // Get the Array of Event Handlers
        var evt = this._getEvent(eventName);

        if (!evt){
            return;
        }

        // Helper Method - an Array.indexOf equivalent
        var getArrayIndex = function(array, item){
            for (var i = array.length; i < array.length; i++){
                if (array[i] && array[i] === item){
                    return i;
                }
            }
            return - 1;
        };

        // Get the Array index of the Event Handler
        var index = getArrayIndex(evt, handler);

        if (index > -1){
            // Remove Event Handler from Array
            evt.splice(index, 1);
        }
    },
    removeAllEventListeners: function(eventName) {
        // Get the Array of Event Handlers
        var evt = this._getEvent(eventName);

        if (!evt){
            return;
        }

        evt.splice(0, evt.length);
    },
    dispatchEvent: function(eventName, eventArgs){
        // Get a function that will call all the Event Handlers internally
        var handler = this._getEventHandler(eventName);
        if (handler){
            // call the handler function
            // Pass in "sender" and "eventArgs" parameters
            handler(this, eventArgs);
        }
    },
    _getEventHandler: function(eventName){
        // Get Event Handler Array for this Event
        var evt = this._getEvent(eventName, false);
        if (!evt || evt.length === 0){
            return null;
        }

        // Create the Handler method that will use currying to
        // call all the Events Handlers internally
        var h = function(sender, args){
            for (var i = 0; i < evt.length; i++){
                evt[i](sender, args);
            }
        };

        // Return this new Handler method
        return h;
    }
};

// Object.prototype.equivalentInObject = function(obj){
	// var out = [];
	// var keys = Object.keys(obj);
	// for(var i = 0; i< keys.length; i++){
		// if(obj[keys[i]] == this){
			// out.push(keys[i]);
		// }
	// }
	// return out;
// }

// alias for object.keys as object.listKeys
Object.listKeys = function(o){
	if(typeof o == 'object'){
		return Object.keys(o);
	}
	return [];
}

/* turns a object of objects structure inside out (use the following example to see what this function does)
*  	x={a:{b:1, c:2}}
* 	Object.invertObjOfObj(x)
*	-> {b:{a:1}, c:{a:2}}
*/
Object.invertObjOfObjs = function(o){
	if(typeof o == 'object'){
		var out = {};
		var xKeys = Object.listKeys(o);
		
		for(var i = 0; i < xKeys.length; i++){
			var yKeys = Object.listKeys(o[xKeys[i]]);
			for(var j = 0; j < yKeys.length; j++){
				if(!out.hasOwnProperty(yKeys[j])){
					out[yKeys[j]] = {};
				}
				if(typeof o[xKeys[i]][yKeys[j]] != 'undefined'){
					out[yKeys[j]][xKeys[i]] = o[xKeys[i]][yKeys[j]];
				}
			}
		}
		
		return out;
	}
}

String.prototype.toDate = function(){
    //var a = this.replace('T','-').replace(/:/g,'-').replace('Z','').split('-');
    //var d = new Date(a[0],a[1],a[2],a[3],a[4],a[5]);
    var d = new Date(this);
    return d;
}

Array.prototype.diff = function(a) {
    return this.filter(function(i) {return !(a.indexOf(i) > -1);});
};

Array.prototype.undefined2Blank = function(){
	for(var i = 0; i < this.length; i++){
		if(typeof this[i] == "undefined"){
			this[i] = "";
		}
	}
	return this;
}

if(!Array.indexOf){
    Array.prototype.indexOf = function(obj){
        for(var i=0; i<this.length; i++){
            if(this[i]==obj){
                return i;
            }
        }
        return -1;
    }
}

Array.prototype.indexOfAll =
  function(obj) {
	var out = [];
    var len = this.length;
	for (var i = 0; i < len; i++) if (this[i] == obj) out.push(i);
	return out;
  };

Array.prototype.removeElement = function (e) {
    var idx = this.indexOf(e);
    while (idx > -1) {
        this.splice(idx, 1); // Remove it if really found!
        var idx = this.indexOf(e);
    }
}

Array.prototype.unique =
  function() {
    var a = [];
    var l = this.length;
    for(var i=0; i<l; i++) {
      for(var j=i+1; j<l; j++) {
        // If this[i] is found later in the array
        if (this[i] === this[j])
          j = ++i;
      }
      a.push(this[i]);
    }
    return a;
  };
  
Array.prototype.max =
  function() {
    var max = this[0];
	var len = this.length;
	for (var i = 1; i < len; i++) if (this[i] > max) max = this[i];
	return max;
  };

Array.prototype.min =
  function() {
    var min = this[0];
	var len = this.length;
	for (var i = 1; i < len; i++) if (this[i] < min) min = this[i];
	return min;
  };

Array.prototype.inArray =
  function(i) {
    if(this.indexOf(i) == -1){
		return false;
	}
	else{
		return true;
	}	
};

Array.prototype.equivalentInArray =
  function(i) {
    for(var j = 0; j < this.length; j++){
		if(JSON.stringify(this[j]) == JSON.stringify(i)){
			return true;
		}
	}
	return false;
};

String.prototype.toHex = function() {
    var str = this;
	var hex = '';
    for(var i=0;i<str.length;i++) {
        hex += ''+str.charCodeAt(i).toString(16);
    }
    return hex;
}
  
Object.size = function(obj) {
    var size = 0, key;
    for (key in obj) {
        if (obj.hasOwnProperty(key)) size++;
    }
    return size;
};

Object.equals = function( x, y ) {
    if ( x === y ) return true;
    // if both x and y are null or undefined and exactly the same

    if ( ! ( x instanceof Object ) || ! ( y instanceof Object ) ) return false;
    // if they are not strictly equal, they both need to be Objects

    if ( x.constructor !== y.constructor ) return false;
    // they must have the exact same prototype chain, the closest we can do is
    // test there constructor.

    for ( var p in x ) {
        if ( ! x.hasOwnProperty( p ) ) continue;
        // other properties were tested using x.constructor === y.constructor

        if ( ! y.hasOwnProperty( p ) ) return false;
        // allows to compare x[ p ] and y[ p ] when set to undefined

        if ( x[ p ] === y[ p ] ) continue;
        // if they have the same strict value or identity then they are equal

        if ( typeof( x[ p ] ) !== "object" ) return false;
        // Numbers, Strings, Functions, Booleans must be strictly equal

        if ( ! Object.equals( x[ p ],  y[ p ] ) ) return false;
        // Objects and Arrays must be tested recursively
    }

    for ( p in y ) {
        if ( y.hasOwnProperty( p ) && ! x.hasOwnProperty( p ) ) return false;
        // allows x[ p ] to be set to undefined
    }
    return true;
}

// Get element index from an array of elements using the event obj
getIndexFromEventObj = function (array,event){
	var elements = array;
	for(var i = 0; i < elements.length; i++){
		if(elements[i] == event.currentTarget){
			return i;
		}
	}
}

function IsIE8Browser() {
    var rv = -1;
    var ua = navigator.userAgent;
    var re = new RegExp("Trident\/([0-9]{1,}[\.0-9]{0,})");
    if (re.exec(ua) != null) {
        rv = parseFloat(RegExp.$1);
    }
    return (rv == 4);
}

function detectIfBrowserSUpported(req){
    var version = 0;
    var browser = null;
    var supported = false;

    var userAgent = navigator.userAgent.toLowerCase();
    $.browser.chrome = /chrome/.test(navigator.userAgent.toLowerCase());

    // Is this a version of IE?
    if($.browser.msie){
        userAgent = $.browser.version;
        userAgent = userAgent.substring(0,userAgent.indexOf('.'));
        version = userAgent;
        browser = 'Internet Explorer';
    }

    // Is this a version of Chrome?
    if($.browser.chrome){
        userAgent = userAgent.substring(userAgent.indexOf('chrome/') +7);
        userAgent = userAgent.substring(0,userAgent.indexOf('.'));
        version = userAgent;
        // If it is chrome then jQuery thinks it's safari so we have to tell it it isn't
        $.browser.safari = false;
        browser = 'Chrome';
    }

    // Is this a version of Safari?
    if($.browser.safari){
        userAgent = userAgent.substring(userAgent.indexOf('safari/') +7);
        userAgent = userAgent.substring(0,userAgent.indexOf('.'));
        version = userAgent;
        browser = 'Safari';
    }

    // Is this a version of Mozilla?
    if($.browser.mozilla){
        //Is it Firefox?
        if(navigator.userAgent.toLowerCase().indexOf('firefox') != -1){
            userAgent = userAgent.substring(userAgent.indexOf('firefox/') +8);
            userAgent = userAgent.substring(0,userAgent.indexOf('.'));
            version = userAgent;
            browser = 'Firefox';
        }
        // If not then it must be another Mozilla
        else{
        }
    }

    // Is this a version of Opera?
    if($.browser.opera){
        userAgent = userAgent.substring(userAgent.indexOf('version/') +8);
        userAgent = userAgent.substring(0,userAgent.indexOf('.'));
        version = userAgent;
        browser = 'Opera';
    }

    supported = isSet(req[browser]) && req[browser].minVersion <= version;

    cookiesSupported = false;

    $.cookie('test_cookie', 'cookie_value', { path: '/' });
    if ($.cookie('test_cookie') == 'cookie_value') {
        cookiesSupported = true;
    }

    trace('Browser information: '+supported+' '+browser+' '+version+' '+cookiesSupported);

    return {
        'supported': supported,
        'browser': browser,
        'version': version,
        'cookiesSupported': cookiesSupported};
}


$(document).ready(function () {

    // Calculating scrollbar size


    var inner = document.createElement('p');
    inner.style.width = "100%";
    inner.style.height = "200px";

    var outer = document.createElement('div');
    outer.style.position = "absolute";
    outer.style.top = "0px";
    outer.style.left = "0px";
    outer.style.visibility = "hidden";
    outer.style.width = "200px";
    outer.style.height = "150px";
    outer.style.overflow = "hidden";
    outer.appendChild (inner);

    document.body.appendChild (outer);
    var w1 = inner.offsetWidth;
    outer.style.overflow = 'scroll';
    var w2 = inner.offsetWidth;
    if (w1 == w2) w2 = outer.clientWidth;

    document.body.removeChild (outer);

    window.SCROLLBAR_WIDTH = (w1 - w2);
});

// This function searches the current URL that the browser is pointed to for URL encoded variables -df
function getParameterByName(name, subsituteUrl)
{
    name = name.replace(/[\[]/, "\\\[").replace(/[\]]/, "\\\]");
    var regexS = "[\\?&]" + name + "=([^&#]*)";
    var regex = new RegExp(regexS);
	if(typeof subsituteUrl != 'undefined'){
		var results = regex.exec(subsituteUrl);
	}
	else{
		var results = regex.exec(window.location.href);
	}
	
	if(results == null)
        return "";
    else
        return decodeURIComponent(results[1].replace(/\+/g, " "));
}

function updateTabSize(){
	$('#tabpage').height('auto');
	var h = $(document).height();
	console.log('document:',h);
	$('#tabpage').height(h);
	console.log('tabPage:',$('#tabpage').height());
}

//Clone Objects
function clone(x){
	function censor(censor) {
	  return (function() {
		var i = 0;

		return function(key, value) {
		  if(i !== 0 && typeof(censor) === 'object' && typeof(value) == 'object' && censor == value) 
			return '[Circular]'; 

		  if(key == 'self' || key == 'parent')
			return '[Circular]'; 
		  
		  // if(i >= 29) // seems to be a harded maximum of 30 serialized objects?
			// return '[Unknown]';

		  ++i; // so we know we aren't using the original object anymore

		  return value;  
		}
	  })(censor);
	}
	var y = ko.toJS(x);
	return JSON.parse(JSON.stringify(y, censor(y)));
}

//Test if objects are equivalent
function equivalent(x,y){
	if(JSON.stringify(clone(x)) == JSON.stringify(clone(y))){
		return true;
	}
	else{
		return false;
	}
}

//Topsis Function
/*
	Requires the following format for input:
	
	xy = {
		designs:["a","b","c"],
		attributes:["x","y","z"],
		optimize:[1,-1,1],
		weights:[1,.5,1.5],
		matrix:[
			[1,2,3],
			[6,4,1],
			[3,1,4]
		]
	};
	
	**Where the row of the matrix are designs and the columns are attributes
*/
function topsis(x){
	this.copy = function(x){
		return JSON.parse(JSON.stringify(x));
	}
	this.normalize = function(){
		var x = this.copy(this.o.matrix);
		var columns = [];
		for(var i = 0; i < x[0].length; i++){
			var sumOfSquares = 0;
			for(var j = 0; j < x.length; j++){
				 sumOfSquares += Math.pow(x[j][i],2);
			}
			columns[i] = Math.pow(sumOfSquares,1/2);
		}
		for(var i = 0; i < x[0].length; i++){
			for(var j = 0; j < x.length; j++){
				x[j][i] = x[j][i] / columns[i];
			}
		}
		this.o.normalized = x;
		return x;
	}
	this.weighted = function(){
		var x = this.copy(this.o.normalized);
		for(var i = 0; i < x[0].length; i++){
			for(var j = 0; j < x.length; j++){
				x[j][i] = x[j][i] * this.o.weights[i];
			}
		}
		this.o.weighted = x;
		return x;
	}
	this.idealPos = function(){
		var x = this.copy(this.o.weighted);
		this.o.idealPos = [];
		for(var i = 0; i < x[0].length; i++){
			for(var j = 0; j < x.length; j++){
				if(this.o.idealPos[i] == undefined){
					this.o.idealPos[i] = x[j][i];
				}
				if(this.o.optimize[i] > 0){
					this.o.idealPos[i] = Math.max(this.o.idealPos[i], x[j][i]);
				}
				else{
					this.o.idealPos[i] = Math.min(this.o.idealPos[i], x[j][i]);
				}
			}
		}
		return this.o.idealPos;
	}
	
	this.idealNeg = function(){
		var x = this.copy(this.o.weighted);
		this.o.idealNeg = [];
		for(var i = 0; i < x[0].length; i++){
			for(var j = 0; j < x.length; j++){
				if(this.o.idealNeg[i] == undefined){
					this.o.idealNeg[i] = x[j][i];
				}
				if(this.o.optimize[i] > 0){
					this.o.idealNeg[i] = Math.min(this.o.idealNeg[i], x[j][i]);
				}
				else{
					this.o.idealNeg[i] = Math.max(this.o.idealNeg[i], x[j][i]);
				}
			}
		}
		return this.o.idealNeg;
	}
	
	this.dist2Pos = function(){
		var x = this.copy(this.o.weighted);
		this.o.dist2Pos = [];
		for(var j = 0; j < x.length; j++){
			var sum = 0;
			for(var i = 0; i < x[j].length; i++){
				sum += Math.pow(x[j][i] - this.o.idealPos[i],2);
			}
			this.o.dist2Pos[j] = Math.pow(sum,1/2);
		}
		return this.o.dist2Pos;
	}
	
	this.dist2Neg = function(){
		var x = this.copy(this.o.weighted);
		this.o.dist2Neg = [];
		for(var j = 0; j < x.length; j++){
			var sum = 0;
			for(var i = 0; i < x[j].length; i++){
				sum += Math.pow(x[j][i] - this.o.idealNeg[i],2);
			}
			this.o.dist2Neg[j] = Math.pow(sum,1/2);
		}
		return this.o.dist2Neg;
	}
	
	this.closeness = function(){
		var x = this.copy(this.o.weighted);
		this.o.closeness = [];
		for(var i = 0; i < x.length; i++){
			this.o.closeness[i] = (this.o.dist2Neg[i]) / (this.o.dist2Neg[i] + this.o.dist2Pos[i]);
		}
		return this.o.closeness;
	}
	
	this.rank = function(){
		var x = this.copy(this.o.designs);
		this.o.rankings = [];
		
		for(var i = 0; i < x.length; i++){
			this.o.rankings.push({
				design : x[i],
				rank : this.o.closeness[i]
			});
		}
		
		this.o.rankings.sort(function(a,b){
			return b.rank - a.rank;
		});
		return this.o.rankings;
	}

	this.o = this.copy(x);
	if(x.designs.length > 0){
	console.log('TOPSIS: normalized matrix',this.normalize());
	console.log('TOPSIS: weighted matrix',this.weighted());
	console.log('TOPSIS: ideal positive vector',this.idealPos());
	console.log('TOPSIS: ideal negative vector',this.idealNeg());
	console.log('TOPSIS: distances to positive ideal',this.dist2Pos());
	console.log('TOPSIS: distances to negative ideal',this.dist2Neg());
	console.log('TOPSIS: score',this.closeness());
	console.log('TOPSIS: rankings',this.rank());
	}
	else{
	
	}
	
	return this.o.rankings;
}

/*
 * A class for formatting number to significant digits.
 * Copyright (C) 2001 Stephen Ostermiller 
 * http://ostermiller.org/contact.pl?regarding?JavaScript+Significant+Figures
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/**
 * An object tho both scans floating points to
 * determine the number of significant figures.
 * and can display a floating point using any number
 * of significant figures.
 *
 * @param s A string representation of a floating point.
 */
function SignificantFigures(s){
    this.order = parseOrder(s);
    this.mantissa = parseMantissa(s);
    this.positive = parseSign(s);

    /**
     * Get the number of significant figures this object uses.
     * Leading zeros are not significant.  Traling zeros up to
     * and after the decimal point are significant.
     * Significant figures is good to know when the number is
     * used in multiplication.
     *
     * @return the number of significant figures.
     */
    this.sigFigs = this.mantissa.length;

    /**
     * Get the least significant decimal this object uses.
     * This is useful to know if a number is being used
     * in addition.
     * 400 - 2 the hundreds place (10^2) is the least significant decimal.
     * 75 - 0 the ones place (10^0) is the least significant decimal.
     * .543 - -3 the 1/1000's place (10^-3) is the least significant decimal.
     *
     * @return an integer representing the least significant decimal place.
     */
    this.sigDecs = ((this.order) - this.mantissa.length);
}

/**
 * Format a floating point for display using the specified 
 * number of significant figures and least significant decimal.
 * Scientific notation may used by this method if this
 * object is very small, very large, has many significant
 * figures, or the number of significant figures would be
 * ambiguous in the output if scientific notation were not
 * used.
 *
 * @param f A floating point number that should be displayed
 * @param sigFigs desired number of significant figures (integer).
 * @param sigDecs the least significant decimal place (integer).
 * @param scientific true iff scientific notation should always be used.
 * @return a string of this object formatted correctly.
 */
function displaySigFigs(f, sigFigs, sigDecs, scientific){
    var s = "" + f;
    var order = parseOrder(s);
    var mantissa = parseMantissa(s);
    var positive = parseSign(s);
    var add;
    var decAdd;
    var sigAdd;
    var zeroScientific=false;
    if (f == 0 || mantissa=="" || mantissa=="0"){
        mantissa = "";
        for (i=0; i<sigFigs; i++){
            mantissa += "0";
        }
        order = sigFigs + sigDecs;
        if (sigDecs < 0 && -sigDecs >= sigFigs){
            zeroScientific = true;
        }
    } else {
        decAdd = ((order - mantissa.length) - sigDecs);
        sigAdd = sigFigs - mantissa.length;
        add = Math.min(sigAdd, decAdd);
        if (add < 0){
            var rounded = round(mantissa, -add);
            if (rounded.length > mantissa.length + add){
                order++;
                if (decAdd > sigAdd){
                	rounded = round(rounded, 1);
                }
            }            
            mantissa=rounded;
        } else if (add > 0){
            for (i=0; i<add; i++){
                mantissa += '0';
            }
        }
        if (mantissa=="" || mantissa=="0"){
            mantissa = "0";
            positive = true;
            order = 1 + sigDecs;
            if (order != 0){
                zeroScientific = true;
            }
        }                          
    }  
    var useScientific = (scientific || mantissa.length > 20 || order > 21 || order < -5 ||
		    (order - mantissa.length > 0 && trailingZeros(mantissa) > 0) || zeroScientific);
    var returnVal = "";    
    if (!positive){
        returnVal += "-";
    }
    if (useScientific) {
        returnVal += mantissa.charAt(0);
        if (mantissa.length > 1){
            returnVal += '.' + mantissa.substring(1, mantissa.length);
        }
        if (order-1!=0){
			returnVal += "e" + (order-1);
        }
    } else {
        var wholePart = "";
        var fractPart = "";
        var needDot = true;
        if (order > 0){
            if (mantissa.length > order){
                wholePart = mantissa.substring(0, order);
                fractPart = mantissa.substring(order, mantissa.length);
            } else {
                wholePart = mantissa;
                needDot = (trailingZeros(mantissa) != 0);
                for(var i=0; i<order-mantissa.length; i++){
                    wholePart += "0";
                }
            }
		} else {
            for(i=0; i<-order; i++){
                fractPart += "0";
			}
            fractPart += mantissa
        }
        returnVal += (
            (wholePart==""?"0":wholePart) + (needDot?".":"") + fractPart

        );
    }
    return (returnVal);
}

/**
 * Count the significant trailing zeros on this object.
 *
 * @return the number of trailing zeros
 */
function trailingZeros(mantissa){
    var zeros = 0;
    for (var i=mantissa.length-1; i>=0; i--){
    	var c = mantissa.charAt(i);
        if (c=='0'){
            zeros++;
        } else {
            return zeros;
        }
    }
    return zeros;
}

/**
 * Parse a string representation of a floating point
 * and pull out the sign.
 *
 * @param s the string representation of a floating point.
 * @return true iff this is a positive number
 */
function parseSign(s){
    var beginning = true;
    var seenDot = false;
    var seenSomething = false;
    var zeros = "";
    var leadZeros = "";
    var all = "";
    var decPlaces = 0;
    var totalDecs = 0;
    var pos = true;
	for (var i=0; i<s.length; i++){
        var c = s.charAt(i);
        if (c>='1' && c<='9'){
            all += zeros + c;
            zeros = "";
            seenSomething = true;
            if (!seenDot){                
                totalDecs++;
                decPlaces++;
            }
            beginning = false;
        } else if (c=='0'){
            if (seenDot){
                if (seenSomething){ 
                    all += zeros + c;
                    zeros = "";
                } else {                                       
                    leadZeros += c;
                    decPlaces--;
				}
            } else {
                totalDecs++;
                if (seenSomething){
                    leadZeros += c;
                    decPlaces++;
                    zeros += c;
                } else {                                       
                    leadZeros += c;
                }
            } 
            beginning = false
		} else if (!seenDot && c=='.'){
            all += zeros;
            zeros = "";
            seenDot=true;
            beginning = false;
		} else if (c=='e' || c=='E' && i+1<s.length){
            var raised = parseInt(s.substring(i+1, s.length));
            decPlaces += raised;
            totalDecs += raised;
            i = s.length;
        } else if (beginning && (c=='+' || c=='-')){
            if (c=='-'){
                pos = !pos;
            }
		}
    }
    if (all == ""){
        return(true);
    } else {
        return(pos);
    }
}

/**
 * Parse a string representation of a floating point
 * and pull out the mantissa.
 *
 * @param s the string representation of a floating point.
 * @return the mantissa of this number.
 */
function parseMantissa(s){
    var beginning = true;
    var seenDot = false;
    var seenSomething = false;
    var zeros = "";
    var leadZeros = "";
    var all = "";
    var decPlaces = 0;
    var totalDecs = 0;
    var pos = true;
	for (var i=0; i<s.length; i++){
        var c = s.charAt(i);
        if (c>='1' && c<='9'){
            all += zeros + c;
            zeros = "";
            seenSomething = true;
            if (!seenDot){                
                totalDecs++;
                decPlaces++;
            }
            beginning = false;
        } else if (c=='0'){
            if (seenDot){
                if (seenSomething){ 
                    all += zeros + c;
                    zeros = "";
                } else {                                       
                    leadZeros += c;
                    decPlaces--;
				}
            } else {
                totalDecs++;
                if (seenSomething){
                    leadZeros += c;
                    decPlaces++;
                    zeros += c;
                } else {                                       
                    leadZeros += c;
                }
            } 
            beginning = false;
		} else if (!seenDot && c=='.'){
            all += zeros;
            zeros = "";
            seenDot=true;
            beginning = false;
		} else if (c=='e' || c=='E' && i+1<s.length){
            var raised = parseInt(s.substring(i+1, s.length));
            decPlaces += raised;
            totalDecs += raised;
            i = s.length;
        } else if (beginning && (c=='+' || c=='-')){
            if (c=='-'){
                pos = !pos;
            }
		}
    }
    if (all == ""){
        return leadZeros;
    } else {
        return all;
    }
}

/**
 * Parse a string representation of a floating point
 * and pull out the exponent.
 *
 * @param s the string representation of a floating point.
 * @return (integer) the number after the e.
 */
function parseOrder(s){
    var beginning = true;
    var seenDot = false;
    var seenSomething = false;
    var zeros = "";
    var leadZeros = "";
    var all = "";
    var decPlaces = 0;
    var totalDecs = 0;
    var pos = true;
	for (var i=0; i<s.length; i++){
        var c = s.charAt(i);
        if (c>='1' && c<='9'){
            all += zeros + c;
            zeros = "";
            seenSomething = true;
            if (!seenDot){                
                totalDecs++;
                decPlaces++;
            }
            beginning = false;
        } else if (c=='0'){
            if (seenDot){
                if (seenSomething){ 
                    all += zeros + c;
                    zeros = "";
                } else {                                       
                    leadZeros += c;
                    decPlaces--;
				}
            } else {
                totalDecs++;
                if (seenSomething){
                    leadZeros += c;
                    decPlaces++;
                    zeros += c;
                } else {                                       
                    leadZeros += c;
                }
            } 
            beginning = false
		} else if (!seenDot && c=='.'){
            all += zeros;
            zeros = "";
            seenDot=true;
            beginning = false;
		} else if (c=='e' || c=='E' && i+1<s.length){
            var raised = parseInt(s.substring(i+1, s.length));
            decPlaces += raised;
            totalDecs += raised;
            i = s.length;
        } else if (beginning && (c=='+' || c=='-')){
            if (c=='-'){
                pos = !pos;
            }
		}
    }
    if (all == ""){
        return totalDecs;
    } else {
    	return decPlaces;
    }
}

/**
 * Remove the specified number of digits from string by 
 * rounding.  Proper rounding rules for scientific purposes 
 * are followed.
 * This method may cause an extra significant figure
 * to be added to the number.  For example, if 999999
 * were rounded, A one would carry over and become
 * a significant figure.  Those who call this method
 * should check for this and call round again if needed.
 *
 * @param mantissa A string representing an whole number of arbitrary length.
 * @param digits A number of digits to remove
 * @return A string represted the rounded version of mantissa
 */
function round(mantissa, digits){
    var last = mantissa.length - digits - 1;
    if (last < 0){
        return("");
    } else if (last >= mantissa.length -1){
        return(mantissa);
	} else {
    	var nextToLast = mantissa.charAt(last+1);
        var lastChar = mantissa.charAt(last);
        var roundUp = false;
        if (nextToLast > '5') { 
            roundUp = true;
        } else if (nextToLast == '5') {
            for (var j=last+2; j<mantissa.length; j++){
                if(mantissa.charAt(j) != '0'){
                    roundUp = true;
                }
            }
            if (lastChar % 2 == 1){
                roundUp = true;
            }
        } 
        var result = "";
        for (var i=last; i>=0; i--){
            var c = mantissa.charAt(i);
            if (roundUp){
                var nextChar;
                if (c == '9'){
                    nextChar = '0';
                } else {
                    switch (c){
                        case '0': nextChar='1'; break;
                        case '1': nextChar='2'; break;
                        case '2': nextChar='3'; break;
                        case '3': nextChar='4'; break;
                        case '4': nextChar='5'; break;
                        case '5': nextChar='6'; break;
                        case '6': nextChar='7'; break;
                        case '7': nextChar='8'; break;
                        case '8': nextChar='9'; break;
                    }
                    roundUp = false;
                }
                result = nextChar + result;
            } else {
                result = c + result;
            }
        }
        if (roundUp){
            result = '1' + result;
        }
        return(result);
    }
}

var htmlEnDeCode = (function() {
    var charToEntityRegex,
        entityToCharRegex,
        charToEntity,
        entityToChar;

    function resetCharacterEntities() {
        charToEntity = {};
        entityToChar = {};
        // add the default set
        addCharacterEntities({
            '&amp;'     :   '&',
            '&gt;'      :   '>',
            '&lt;'      :   '<',
            '&quot;'    :   '"',
            '&#39;'     :   "'"
        });
    }

    function addCharacterEntities(newEntities) {
        var charKeys = [],
            entityKeys = [],
            key, echar;
        for (key in newEntities) {
            echar = newEntities[key];
            entityToChar[key] = echar;
            charToEntity[echar] = key;
            charKeys.push(echar);
            entityKeys.push(key);
        }
        charToEntityRegex = new RegExp('(' + charKeys.join('|') + ')', 'g');
        entityToCharRegex = new RegExp('(' + entityKeys.join('|') + '|&#[0-9]{1,5};' + ')', 'g');
    }

    function htmlEncode(value){
        var htmlEncodeReplaceFn = function(match, capture) {
            return charToEntity[capture];
        };

        return (!value) ? value : String(value).replace(charToEntityRegex, htmlEncodeReplaceFn);
    }

    function htmlDecode(value) {
        var htmlDecodeReplaceFn = function(match, capture) {
            return (capture in entityToChar) ? entityToChar[capture] : String.fromCharCode(parseInt(capture.substr(2), 10));
        };

        return (!value) ? value : String(value).replace(entityToCharRegex, htmlDecodeReplaceFn);
    }

    resetCharacterEntities();

    return {
        htmlEncode: htmlEncode,
        htmlDecode: htmlDecode
    };
})();