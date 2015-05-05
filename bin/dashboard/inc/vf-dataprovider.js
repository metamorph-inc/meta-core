/**
 *  vf_dataprovider
 *
 *  The master "class" for the dataprovider
 */
function vf_dataprovider() {
    console.log("vf_dataprovider: new");
    console.log("vf_dataprovider: cookie: _session_id");
    
    var self = this;
	this.serverBased = ko.observable(false);		
    var state = null;
    this.designHash = '';
    this.stateChanged = ko.observable(0);
	this.topsisResults = ko.observable([]);
    this.showExperimental = ko.observable(false);
    //this.icolor = d3.interpolateHsl("#ff3300","#1f77b4");
	this.theme = ko.observable(getParameterByName('theme', document.originalURL));
	this.themes = ko.observableArray(['light','dark','blue']);
	this.subTitle = ko.observable('[FANG: SURVIVABILITY / HUMAN FACTORS CHALLENGE] | BETA TESTING');
	this.vf_neighborhood = ko.observable(parent.location.pathname.split('/')[1]); // get neighborhood from url
	this.vf_project = ko.observable(parent.location.pathname.split('/')[2]); // get project or team from url
	this.disableLeaderBoard = ko.observable(true);

	this.icolor = function(n){
		var a = "#ff3300";
		var b = "#1f77b4";

		var c = d3.hsl(a);
		var d = d3.hsl(b);

		var e = d3.interpolateNumber(c.h,d.h);
		var f = d3.interpolateNumber(c.s,d.s);
		var g = d3.interpolateNumber(c.l,d.l);

		c.h = e(n);
		c.s = f(n);
		c.l = g(n);

		return c.rgb().toString();
	}

	/**
     *  vfMode
     *
     *  type: computed
     *
     *  output: returns true if the dashboard is executed from vehicle forge
     *
     */
    this.vfMode = ko.computed(function () {
		// Setup the location for files
		/// create global variable projectLocation
		var api_url = getParameterByName('api_url', document.originalURL);
		var env = getParameterByName('env', document.originalURL);
		var resource_url = getParameterByName('resource_url', document.originalURL);
		var format = getParameterByName('format', document.originalURL);

		// on VF with latested API
		if(api_url !== '' && env === 'vf'){
			projectLocation = api_url;
			return true;
		}
		
		// on VF using legacy API
		if(resource_url !== '' && format !== ''){
			projectLocation = '/' + self.vf_neighborhood() + '/' + self.vf_project() + '/home/design_api/monolith/design_project?resource_url=' + resource_url;
			//projectLocation = resource_url;
			return true;
		}

		// running local
		if(resource_url !== '' && format === ''){
			projectLocation = resource_url;
			return false;
		}

		///	Catch All vfMode is false, localMode is false, using 'project/manifest.project.json'
		projectLocation = parent.location.pathname.replace("dashboard.html","") + "project/manifest.project.json";
		return false;
		
	}, this);
	
	this.setTheme = ko.computed(function () {
		var path1 = './css/'+ this.theme() +'/main.css';
		var path2 = './css/'+ this.theme() +'/vf-widgets.css'
		
		$('#main_css').attr('href',path1);
		$('#widget_css').attr('href',path2);
		
	}, this);
  
    /**
     *  allConfigurations
     *
     *  type: observable array
     *
     *  This array contains all the raw (unfiltered) configurations loaded from files
     */
	this.allConfigurations = ko.observableArray([]);
	
    /**
     *  componets
     *
     *  type: observable array
     *
     *  This array contains all the components loaded
     */
	this.componetsRaw = ko.observableArray([]);
	
	/**
     *  designSpaceModels
     *
     *  type: observable array
     *
     *  This array contains all the components loaded
     */
	this.designSpaceModels = ko.observableArray([]);
	
    /**
     *  componets
     *
     *  type: observable array
     *
     *  This array contains all the components loaded
     */
	this.componets = ko.computed(function () {
			var data1 = this.componetsRaw();
			var data2 = this.designSpaceModels();
			for(var i = 0; i < data1.length; i++){
				for(var j = 0; j < data1[i].componets.length; j++){
					for(var k = 0; k < data2.length; k++){
						for(var l = 0; l < data2[k].componets.length; l++){
							/*if(isSet('data1[i].componets[j].typeId', {data1:data1, i:i, j:j})){
								if(data2[k].componets[l].instanceId === data1[i].componets[j].id && data2[k].componets[l].componetId === data1[i].componets[j].typeId){
									data1[i].componets[j].name = data2[k].componets[l].name;
									data1[i].componets[j].type = data2[k].componets[l].type;
									data1[i].componets[j].containerType = data2[k].componets[l].containerType;
									data1[i].componets[j].typeId = data2[k].componets[l].typeId;
									break;
								}
							}
							else{*/
								if(data2[k].componets[l].id === data1[i].componets[j].id){
									data1[i].componets[j].name = data2[k].componets[l].name;
									data1[i].componets[j].type = data2[k].componets[l].type;
									data1[i].componets[j].typeId = data2[k].componets[l].typeId;
									data1[i].componets[j].containerType = data2[k].componets[l].containerType;
									break;
								}
							//}
						}
					}
				}
			}
			return data1;
		}, this).extend({
			throttle : 100
		});
    
    /**
     *  componetTypes
     *
     *  type: computed
     *
     *  output: list of component types
     *
     *  This is a computed function that returns a list of all
     *  component types.
     */
	this.componetTypes = ko.computed(function () {
			var out = [];
			var t = this.componets();
			for(var i = 0; i < this.componets().length; i++){
				for(var j = 0; j < this.componets()[i].componets.length; j++){
					if(!out.inArray(this.componets()[i].componets[j].type + ' (' + this.componets()[i].componets[j].containerType + ')')){
						out.push(this.componets()[i].componets[j].type + ' (' + this.componets()[i].componets[j].containerType + ')');
					}
				}
			}
			out.sort();
			return out;
		}, this).extend({
			throttle : 100
		});
    
    /**
     *  componetsAvailble
     *
     *  type: computed
     *
     *  output: boolean - true=components are available
     *
     *  This is a computed function that returns a boolean value
     *  that indicates whether the components have been loaded.
     */
	this.componetsAvailble = ko.computed(function () {
		if(this.componets().length > 0){
			return true;
		}
		else{
			return false;
		}
	},this);
    
    /**
     *  configurationsAvailble
     *
     *  type: computed
     *
     *  output: boolean - true=configurations are available
     *
     *  This is a computed function that returns a boolean value
     *  that indicates whether the configurations have been loaded.
     */
	this.configurationsAvailble = ko.computed(function () {
		if(this.allConfigurations().length > 0){
			return true;
		}
		else{
			return false;
		}
	},this);
	

    
    
    /**
     *  configurations
     *
     *  type: computed boolean
     *
     *  output: boolean - true=scores are available
     *
     *  This is a computed function that returns a boolean value
     *  that indicates whether the scores have been loaded.
     *  Functions as a filter on the self.allConfigurations array.
     */
	 
	function IniAnnotate() 
	{
		var currDate = new Date();
		
		var obj = [{
			'user' : 'User001',
			'addedDate' : currDate,
			'lastModDate' : currDate,
			'text' : ""
		}];
		
		return ko.observableArray(obj);
	}
		
	this.configurations = ko.computed(function () {
    
		// Initialize out to contain all configurations (this is a reference copy - maybe change to copy to preserve allConfigurations array)
		//var out = clone(this.allConfigurations());
		var out = this.allConfigurations();

		var currDate = new Date();
		
		for (var i = out.length - 1; i >= 0; i--)
		{
			out[i].limit_color = "#1f77b4";
			out[i].score_color = "#555555";
			out[i].requirement_color = "#555555";				
			out[i].isActive = ko.observable(out[i].isActive);
			out[i].isSelected = ko.observable(out[i].isSelected);
			out[i].isCompareSelected = ko.observable(false);
			out[i].annotate = ko.observableArray([{
				'user' : 'User001',
				'addedDate' : currDate,
				'lastModDate' : currDate,
				'text' : ""
			}]);
			
			out[i].isSelected.subscribe(function (newValue) {
				$(window).trigger('vf-selectionChange');
			});

			out[i].color = ko.observable("#1f77b4");
		}
		
		// Loop over list in out - for a particular design and testBench, keep only the latest
		// Only filter if not running from VF
		if(!self.vfMode()){
			for (var i = out.length - 1; i >= 0; i--) {
				if(out[i] !== null){
					for (var j = out.length - 1; j >= 0; j--) {
						if(out[j] !== null){
							
							// check if configuration is a repeat
							if(typeof out[i].time.toDate() !== 'undefined' && typeof out[j].time.toDate() !== 'undefined'){
								if (out[i].designId == out[j].designId && out[i].testBench == out[j].testBench && out[i].time.toDate() < out[j].time.toDate()) {									
									out.splice(i, 1, null);
									j = out.length - 1;
								}
								if (out[i].designId == out[j].designId && out[i].testBench == out[j].testBench && out[i].time.toDate() > out[j].time.toDate()) {
									out.splice(j, 1, null);
									j = out.length - 1;
								}
							}
						}
					}
				}
			}
			//remove nulls from filtering process
			for (var i = out.length - 1; i >= 0; i--) {
				if(out[i] === null){
					out.splice(i, 1);
				}
			}
		}
		
		// look for multiple designs with the same name
		// append last four characters of designId to name to differentiate designs
		for (var i = out.length - 1; i >= 0; i--) {
			var appendId = false;
			for (var j = out.length - 1; j >= 0; j--) {
				if(out[i].design == out[j].design && out[i].designId != out[j].designId){
					appendId = true;
					out[j].design += " #" + out[j].designId.slice(-4);
				}
			}
			if(appendId){
				for (var j = out.length - 1; j >= 0; j--) {
					if(out[i].designId == out[j].designId){
						out[j].design += " #" + out[j].designId.slice(-4);
					}
				}
			}
			appendId = false;
		}
		
		
		// compile PET function
		var petFunction = function(tb, petResults, time, put){
			var testBenchObjExists = false
			for(var i = 0; i < put.length; i++){
				if(put[i].name === tb){
					testBenchObjExists = true;
					break;
				}
			}
			if(testBenchObjExists){
				put[i].PETResults = {
					time: time,
					responses: petResults
				};
			}
			else{
				put.push({
					name: tb,
					PETResults: {
						time: time,
						responses: petResults
					}
				});
			}
		}
		
		for (var i = 0; i < out.length; i++) {
			// create array for testBench
			if(isSet('Array.isArray(out[i].testBench)',{out:out,i:i}) && !Array.isArray(out[i].testBench)){
				var tempTestBench = out[i].testBench;
				out[i].testBench = [];
				petFunction(tempTestBench, out[i].responses, out[i].responsesTimeStamp, out[i].testBench);
			}
			
			//cleanup
			delete out[i].responses;
			delete out[i].responsesTimeStamp;
			delete out[i].PCCResults;
			delete out[i].PCCResultsTimeStamp;
			//delete out[i].time;
			
			for (var j = 0; j < out.length; j++) {
				if (out[i].design == out[j].design && out[i].testBench != out[j].testBench) {
					//transfer variables
					for (var k = 0; k < out[j].variables.length; k++) {
						out[i].variables.push(out[j].variables[k]);
					}
					petFunction(out[j].testBench, out[j].responses, out[j].responsesTimeStamp, out[i].testBench);						
					
					
					
					// Delete the second test bench entry
					out.splice(j, 1);
					i = 0;
					j = 0;
				}
			}
		}
		
		
		// Compute the id value and add to each entry
		var tid = "";
		for (var i = 0; i < out.length; i++) {
			tid = out[i].design+out[i].time;
			out[i].id = 'cid' + tid.toHex();
		}
		
		// sort array
		out.sort(function(a,b){
			if([a.design,b.design].sort().indexOf(a.design)==1){
				return 1;
			}
			else{
				return -1;
			}
		});
  
		  // Set the color for the configuration based on whether there is a limit exceeded condition
		  for (i=0; i<out.length; i++) {
			out[i].limit_color = "#1f77b4";
			// Check for a limit exceeded condition
			for (j=0; j<out[i].variables.length; j++) {
				if (out[i].variables[j].type === "limit") {
					out[i].limit_color = "#ff3300";
					break;
				}
			}
		  }

		return out;
	}, this).extend({
		throttle : 100
	});


	/**
	*    Set the color scheme
	*    0 - default, 1 - limit, 2 - score, 3 - ranking, 4 - Requirements, 5 - Pcc, 6 - Components, 7 - Compare
	*/
	this.colorScheme = ko.observable(0);

	/**
	*    Set default colors in configurations
	*/
	this.setColorsForDefault = ko.computed(function () {
		if(self.colorScheme() === 0){
			for(var i = 0; i < self.configurations().length; i++){
				self.configurations()[i].color("#1f77b4")
			}
		}
		
	}, this);

	/**
	*    Set default colors in configurations
	*/
	this.setColorsForScores = ko.computed(function () {
		if(self.colorScheme() === 2){
			for(var i = 0; i < self.configurations().length; i++){
				self.configurations()[i].color(self.configurations()[i].score_color);
			}
		}
		
	}, this);

	/**
	*    Set default colors in configurations
	*/
	this.setColorsForRankings = ko.computed(function () {
		if(self.colorScheme() === 3){
			for(var i = 0; i < self.configurations().length; i++){
				if (self.topsisResults().length === 0) {
					self.configurations()[i].color("#1f77b4");
					break;
				}
				var set = false;
				for (j=0; j<self.topsisResults().length; j++) {
					if (self.configurations()[i].design === self.topsisResults()[j].design) {
						set = true;
						self.configurations()[i].color(self.icolor(self.topsisResults()[j].rank));
						break;
					}
				}
				if(!set){
					self.configurations()[i].color("#555555");
				}
			}
		}
		
	}, this);

	/**
	*    Set default colors in configurations
	*/
	this.setColorsForLimits = ko.computed(function () {
		if(self.colorScheme() === 1){
			for(var i = 0; i < self.configurations().length; i++){
				self.configurations()[i].color(self.configurations()[i].limit_color);
			}
		}
		
	}, this);

	/**
	*    Set default colors in configurations
	*/
	this.setColorsForRequirements = ko.computed(function () {
		if(self.colorScheme() === 4){
			for(var i = 0; i < self.configurations().length; i++){
				self.configurations()[i].color(self.configurations()[i].requirementsResults);
			}
		}
		
	}, this);

	/**
	*    Set default colors in configurations
	*/
	this.setColorsForComponents = ko.computed(function () {
		if(self.colorScheme() === 6){

		}
		
	}, this);

	/**
	*    Set default colors in configurations
	*/
	this.setColorsForPcc = ko.computed(function () {
		if(self.colorScheme() === 5){
			for(var i = 0; i < self.configurations().length; i++){
				if (self.pccResults().length === 0) {
					self.configurations()[i].color("#1f77b4");
					break;
				}
				var set = false;
				for (j=0; j<self.pccResults().length; j++) {
					if (self.configurations()[i].design === self.pccResults()[j].design) {
						set = true;
						self.configurations()[i].color(self.icolor(self.pccResults()[j].PCCResults.byAttr.JointPCC[Object.listKeys(self.pccResults()[j].PCCResults.byAttr.JointPCC)[0]]));
						break;
					}
				}
				if(!set){
					self.configurations()[i].color("#555555");
				}
			}
		}
		
	}, this);

	/**
	*    Set default colors in configurations
	*/
	this.setColorsForCompare = ko.computed(function () {
		if(self.colorScheme() === 7){
			for(var i = 0; i < self.configurations().length; i++){
				if(self.configurations()[i].isCompareSelected()){
					self.configurations()[i].color("#1f77b4");
				}
				else{
					self.configurations()[i].color("#555555");
				}
			}
		}
		
	}, this);
		
	 /**
     *  pccResults
     *
     *  type: observable array
     *
     *  output: the new, "flat" PCC data structure
     *
     */
	 this.pccResults = ko.observableArray();
	 this.allPccResults = ko.observableArray([]);
	 this.activePccResultsIds = ko.observableArray([]);  // this will be deleted
	 this.activeUpPccResultsIds = ko.observableArray([]);
	 this.activeSaPccResultsIds = ko.observableArray([]);
	 this.pccListOfUpMethods = ['TS','MCS','FFNI','UDR'];
	 this.pccListOfSaMethods = ['FAST','EFAST','SOBOL'];
	 this.activePccResults = ko.computed(function(){
		var out = [];
		for(var i = 0; i < self.activeUpPccResultsIds().length; i++){
			var pccObj = {
				PCCResults: {
					byMethod: {},
					byAttr: {}
				},
				distExist : false,
				sensExist : false,
				upTime: null,
				saTime: null
			};

			if(self.activeUpPccResultsIds()[i] !== null && self.activeUpPccResultsIds()[i] !== undefined){
				var up = self.allPccResults()[i][self.activeUpPccResultsIds()[i]];
				pccObj.design = up.design;
				pccObj.designId = up.designId;
				pccObj.upTime = up.time;
				pccObj.testBench = up.testBench;
				pccObj.distExist = up.distExist;
				var attr = Object.listKeys(up.PCCResults.byMethod)[0];
				pccObj.PCCResults.byMethod[attr] = up.PCCResults.byMethod[attr];
				pccObj.variables = up.variables;
			}

			if(self.activeSaPccResultsIds()[i] !== null && self.activeSaPccResultsIds()[i] !== undefined){
				var sa = self.allPccResults()[i][self.activeSaPccResultsIds()[i]];
				pccObj.design = sa.design;
				pccObj.designId = sa.designId;
				pccObj.saTime = sa.time;
				pccObj.testBench = sa.testBench;
				pccObj.sensExist = sa.sensExist;
				var attr = Object.listKeys(sa.PCCResults.byMethod)[0];
				pccObj.PCCResults.byMethod[attr] = sa.PCCResults.byMethod[attr];
				pccObj.variables = sa.variables;
			}

			pccObj.PCCResults.byAttr = Object.invertObjOfObjs(pccObj.PCCResults.byMethod);
			
			out.push(pccObj);
		}
		this.pccResults(out);
		return out;
	},this);

	 this.pccContextMenuDictionary = ko.computed(function(){
	 	var out = {};

	 	for(var i = 0; i < self.allPccResults().length; i++){
	 		for(var j = 0; j < self.allPccResults()[i].length; j++){
	 			
	 			if(!isSet('out[self.allPccResults()[i][j].design]',{'self':self, 'i':i, 'j':j, 'out':out})){
	 				out[self.allPccResults()[i][j].design] = {};
	 			}
	 			if(!isSet('out[self.allPccResults()[i][j].design][self.allPccResults()[i][j].testBench]',{'self':self, 'i':i, 'j':j, 'out':out})){
	 				out[self.allPccResults()[i][j].design][self.allPccResults()[i][j].testBench] = {};
	 				out[self.allPccResults()[i][j].design][self.allPccResults()[i][j].testBench]['up'] = {};
	 				out[self.allPccResults()[i][j].design][self.allPccResults()[i][j].testBench]['sa'] = {};
	 			}
	 			if(isSet('Object.listKeys(self.allPccResults()[i][j].PCCResults.byMethod)', {'self':self, 'i':i, 'j':j})){
	 				var keys = Object.listKeys(self.allPccResults()[i][j].PCCResults.byMethod);
	 				for(var k = 0; k < keys.length; k++){
	 					if(self.pccListOfUpMethods.inArray(keys[k])){
	 						out[self.allPccResults()[i][j].design][self.allPccResults()[i][j].testBench]['up'][keys[k] + ' ' + self.allPccResults()[i][j].time] = {'index':i, 'value':j};
	 					}
	 					if(self.pccListOfSaMethods.inArray(keys[k])){
	 						out[self.allPccResults()[i][j].design][self.allPccResults()[i][j].testBench]['sa'][keys[k] + ' ' + self.allPccResults()[i][j].time] = {'index':i, 'value':j};
	 					}
	 				}
	 			}
	 		}
	 	}
	 	return out;
	 },this).extend({
			throttle : 100
		});

	 this.currentPccResultIndex = ko.observable(0);
	 
	 this.currentPccResultObject = ko.computed(function(){
	 	return self.activePccResults()[self.currentPccResultIndex()];
	 },this).extend({
		throttle : 100
		});
	 
	 this.currentPccUpMethod = ko.computed(function(){
	 	if(isSet('self.currentPccResultObject().PCCResults.byMethod',{self:self})){
		 	var currentMethods = Object.listKeys(self.currentPccResultObject().PCCResults.byMethod);
		 	for(var i = 0; i < currentMethods.length; i++){
		 		if(self.pccListOfUpMethods.inArray(currentMethods[i].split(" ")[0])){
		 			return currentMethods[i].split(" ")[0];
		 		}
		 	}
		 }
	 	return null;
	 },this).extend({
		throttle : 100
		});
	 
	 this.currentPccSaMethod = ko.computed(function(){
		 	if(isSet('self.currentPccResultObject().PCCResults.byMethod',{self:self})){
		 	var currentMethods = Object.listKeys(self.currentPccResultObject().PCCResults.byMethod);
		 	for(var i = 0; i < currentMethods.length; i++){
		 		if(self.pccListOfSaMethods.inArray(currentMethods[i].split(" ")[0])){
		 			return currentMethods[i].split(" ")[0];
		 		}
		 	}
		 }
	 	return null;
	 },this).extend({
		throttle : 100
		});


 	this.pccUpContextMenuHtml = ko.computed(function(){
	 	if(isSet('self.currentPccResultObject().PCCResults.byMethod',{self:self})){
		 	
		 	var out = [{'text':'UP Methods Available','index':null,'value':null, 'check':false, 'type':null, 'divider':false}];
		 	var currentMethods = Object.listKeys(self.currentPccResultObject().PCCResults.byMethod);
		 	var availableMethods = Object.listKeys(self.pccContextMenuDictionary()[self.currentPccResultObject().design][self.currentPccResultObject().testBench]['up']);
		 	var availableMethodsIds = self.pccContextMenuDictionary()[self.currentPccResultObject().design][self.currentPccResultObject().testBench]['up'];
		 	for(var i = 0; i < availableMethods.length; i++){
		 		// if the ith available method matches either of the two current methods then it must have a check in front
		 		if(availableMethods[i] === currentMethods[0] + ' ' + self.currentPccResultObject().upTime || availableMethods[i] === currentMethods[1] + ' ' + self.currentPccResultObject().upTime){
		 			out.push({
		 				'text':availableMethods[i],
		 				'index':availableMethodsIds[availableMethods[i]].index,
		 				'value':availableMethodsIds[availableMethods[i]].value,
		 				'check':true,
		 				'type':'up', 
		 				'divider':false
		 			});
		 		}
		 		else{
		 			out.push({
		 				'text':availableMethods[i],
		 				'index':availableMethodsIds[availableMethods[i]].index,
		 				'value':availableMethodsIds[availableMethods[i]].value,
		 				'check':false,
		 				'type':'up', 
		 				'divider':false
		 			});
		 		}
		 	}
		 }
	 	return out;
	 },this).extend({
		throttle : 100
		});

	 this.pccSaContextMenuHtml = ko.computed(function(){
	 	if(isSet('self.currentPccResultObject().PCCResults.byMethod',{self:self})){
		 	
		 	var out = [{'text':'SA Methods Available','index':null,'value':null, 'check':false, 'type':null, 'divider':false}];
		 	var currentMethods = Object.listKeys(self.currentPccResultObject().PCCResults.byMethod);
		 	var availableMethods = Object.listKeys(self.pccContextMenuDictionary()[self.currentPccResultObject().design][self.currentPccResultObject().testBench]['sa']);
		 	var availableMethodsIds = self.pccContextMenuDictionary()[self.currentPccResultObject().design][self.currentPccResultObject().testBench]['sa'];
		 	for(var i = 0; i < availableMethods.length; i++){
		 		// if the ith available method matches either of the two current methods then it must have a check in front
		 		if(availableMethods[i] === currentMethods[0] + ' ' + self.currentPccResultObject().saTime || availableMethods[i] === currentMethods[1] + ' ' + self.currentPccResultObject().saTime){
		 			out.push({
		 				'text':availableMethods[i],
		 				'index':availableMethodsIds[availableMethods[i]].index,
		 				'value':availableMethodsIds[availableMethods[i]].value,
		 				'check':true,
		 				'type':'sa', 
		 				'divider':false
		 			});
		 		}
		 		else{
		 			out.push({
		 				'text':availableMethods[i],
		 				'index':availableMethodsIds[availableMethods[i]].index,
		 				'value':availableMethodsIds[availableMethods[i]].value,
		 				'check':false,
		 				'type':'sa', 
		 				'divider':false
		 			});
		 		}
		 	}
		 }
	 	return out;
	 },this).extend({
		throttle : 100
		});

	 this.pccContextMenuHtml = ko.computed(function(){
	 	var out = [];
	 	return out.concat(self.pccUpContextMenuHtml(),[{'text':'','index':null,'value':null, 'check':false, 'type':null, 'divider':true}],self.pccSaContextMenuHtml());
	 },this).extend({
		throttle : 100
		});


	/**
     *  pccUPMethodsAvailable
     *
     *  type: observable array
     *
     *  output: what pcc methods are available for UP analysis for the selected testbench and config
     *
     */
	this.pccUPMethodsAvailable = ko.observableArray();
	
	/**
     *  pccUPMethodSelected
     *
     *  type: observable
     *
     *  output: what pcc UP method has been selected for the selected testbench and config
     *
     */
	this.pccUPMethodSelected = ko.observable();
	
	/**
     *  pccSAMethodsAvailable
     *
     *  type: observable array
     *
     *  output: what pcc methods are available for SA analysis for the selected testbench and config
     *
     */
	this.pccSAMethodsAvailable = ko.observableArray();
	
	/**
     *  pccSAMethodSelected
     *
     *  type: observable
     *
     *  output: what pcc SA method has been selected for the selected testbench and config
     *
     */
	this.pccSAMethodSelected = ko.observable();
	
	/**
     *  changeComplareSelectedStatus
     *
     *  type: function
     *
     *  output: none
     *
     *	This function takes in a configuration design name and it's status for isCompareSelected,
	 *  then it finds the design in the configurations object and changes it's status
     */
	this.changeCompareSelectedStatus = function(config, status) {
		for(var i = 0; i < this.configurations().length; i++)
		{
			if (this.configurations()[i].design == config)
			{
				this.configurations()[i].isCompareSelected(status);
			}
		}
	}
	
	/**
     *  numberOfConfigurations
     *
     *  type: computed
     *
     *  output: Number of Configurations
     *
     */
	this.numberOfConfigurations = ko.computed(function () {
			return this.configurations().length;
		}, this);
    
	/**
     *  listOfConfigurations
     *
     *  type: computed array
     *
     *  This is a computed function that returns a list of the
     *  names of the configurations as returned by the self.configurations()
     *  computed array.
     *  Functions as a filter on the self.configuratons() generated array.
     */
	this.listOfConfigurations = ko.computed(function () {
			var out = [];
			for (var i = 0; i < this.configurations().length; i++) {
				out.push(this.configurations()[i].design);}
			return out;
		}, this).extend({
			throttle : 100
		});
	
	 /**
     *  selectedConfiguration
     *
     *  type: observable
     *
     *  Returns the first entry from self.listOfConfigurations() list.
     */
	this.selectedConfiguration = ko.observable(this.listOfConfigurations()[0]);
	this.selectedConfigurationInfo = ko.computed(function(){
		
		// last selected configuration
		var out = {};
		var annotates =[];
			var lsc = this.configurations()[this.listOfConfigurations().indexOf(this.selectedConfiguration())];
			if(typeof lsc != 'undefined'){
					out = {
						did: lsc.designId,
						id: lsc.id,
						time: lsc.time,
						annotates: lsc.annotate //[]
						};
					//annotates.push({GroupTitle: "Annotates", GroupBody: lsc.annotate});
					//annotates.push("annotate2");
					//annotates.push("annotate3");
			}
		
			return out;
	},this);

	
	/**
	* Context menu 
	*
	*
	*/
	//this.contextAnnotateDictionary = ko.observable();
	this.contextAnnotateCurrrent = ko.computed(function() {
/* 		if (typeof this.contextAnnotateDictionary()[this.selectedConfiguration()] != "undefined")
		{
			return this.contextAnnotateDictionary()[this.selectedConfiguration()];
		}
		else
		{
			return [];
		} */
		//this.configurations;
		this.selectedConfiguration();
		
		var out = [];
		for (var i = 0; i < this.configurations().length; i++)
		{
			if(this.configurations()[i].design == this.selectedConfiguration())
			{
				out = this.configurations()[i].annotate;
			}
		}
		
		
		return out;
	},this);

	/**
     *  listOfCompareConfigurationsAvailable
     *
     *  type: function
     *
     *  This is a computed function that returns a list of the
     *  names of the configurations as returned by the self.configurations()
     *  computed array.  The only difference between this and the listOfConfigurations
	 *  computed is a blank first element
     *  Functions as a filter on the self.configuratons() generated array.
     */
		 
	 this.listOfCompareConfigurationsAvailable = ko.computed(function () {
			var out = [];
			
			out = clone(this.listOfConfigurations());
			out.splice(0,0," ");
			
			return out;
		}, this).extend({
			throttle : 100
		});

	this.selectDesignForCompare = function(){
		if (jQuery.inArray(self.selectedConfiguration(), self.listOfCompareConfigurations()) == -1)
		{
			var compareArray = self.listOfCompareConfigurations();
			compareArray.push(self.selectedConfiguration());
			self.listOfCompareConfigurations(compareArray);
			self.changeCompareSelectedStatus(self.selectedConfiguration(), true);
		}
	};

	this.removeDesignFromCompare = function(config){
		//change its compare status of false
		self.changeCompareSelectedStatus(config, false);
		
		//remove it from the compare list
		self.listOfCompareConfigurations().splice(jQuery.inArray(config, self.listOfCompareConfigurations()), 1);
		
		//invoke the datatable to rerender...
		self.listOfCompareConfigurations(self.listOfCompareConfigurations());
	};
		
	/**
	* Design Space Options 
	*/
	this.designspaceOptions = ko.computed(function () {

		var out = {
					headers : ['Designs'],
					data : []
		};
				
		var typeIds = [];
			for(var i = 0; i < this.designSpaceModels().length; i++){
				for(var j = 0; j < this.designSpaceModels()[i].componets.length; j++){
					if(!typeIds.inArray(this.designSpaceModels()[i].componets[j].type + this.designSpaceModels()[i].componets[j].typeId)){
						typeIds.push(this.designSpaceModels()[i].componets[j].type + this.designSpaceModels()[i].componets[j].typeId);
						out.headers.push(this.designSpaceModels()[i].componets[j].type + ' (' + this.designSpaceModels()[i].componets[j].containerType + ')');
					}
				}
			}
			
			// create an array[row][col][entry] for each cell in the table
			var data = [];
			for(var i = 0; i < this.listOfConfigurations().length; i++){
				// create [row] for configuration
				data.push([]);
				
				// find configuration in componets()
				for(var j = 0; j < this.componets().length; j++){
					// match based on design name
					if(this.listOfConfigurations()[i] === this.componets()[j].design){
						for(var k = 0; k < typeIds.length; k++){
							// create [col] and [entry] for configuration
							data[i].push([]);
							for(var l = 0; l < this.componets()[j].componets.length; l++){
								if(typeIds[k] === (this.componets()[j].componets[l].type + this.componets()[j].componets[l].typeId)){
									data[i][k].push(this.componets()[j].componets[l].name);
								}
							}
						}
					}
				}
			}
			
			// generate out.data based on data[row][col][entry]
			for(var i = 0; i < this.listOfConfigurations().length; i++){
				// the first col is the design name
				out.data.push([this.listOfConfigurations()[i]]);
				
				
				for(var j = 0; j < typeIds.length; j++){
					if(isSet('data[i][j]',{data:data,i:i,j:j})){
						data[i][j].sort();
						if(data[i][j].length < 2){
							if(data[i][j][0] === undefined){
								out.data[i].push("n/a");
							}
							else{
								out.data[i].push(data[i][j][0]);
							}
						}
						else{
							out.data[i].push("compound");
						}
					}
					else{
						out.data[i].push("n/a");
					}
				}
			}
			var comps = [];
			//calculates the position of the compounds
			for(var i = 0; i< out.headers.length; i++) {
				if(out.headers[i].indexOf("Compound") != -1) {
					comps.push(i);
				}
			}
			comps.reverse();
			//removes compounds from data
			for(var i = 0; i<out.data.length; i++) {
				for(var j = 0; j<comps.length; j++) {
					out.data[i].splice(comps[j],1);
				}
			}
		var data2 = []
			for (var i = 0; i < out.data.length; i++) {
				// data2[i] = [{rad: 20, config: "test"}, data2[i]];
				data2[i] = {
					rad: 20,
					config: out.data[i][0],
					comps: out.data[i].slice(1, out.data[i].length),
					color: 20
				};
			}
			out.data = data2;
			
			var heads = [];
			for(var i = 0; i<out.headers.length; i++) {
				var count = 0;
				if(out.headers[i].indexOf("Compound") == -1){
					for(var j = 0; j<data.length; j++) {
						if(data[j][i] != ""){
							count = count + 1;
						}
					}
					if (count>0){
						heads.push(out.headers[i]);
					}
				}
			}
			out.headers = heads.slice(1,heads.length);
			out.headers.unshift("All Designs"); 
			
			var temp = [];
			for(var i = 0; i<out.headers.length; i++){
				out.data[i] = i;
				temp[i] = {headers: out.headers[i], data: i};
			}
			out = temp;
		return out;
	}, this).extend({
		throttle: 100
	});
    
		
    /**
     *  activeConfigurations
     *
     *  type: computed array
     *
     *  This is a computed function that returns a list of the
     *  configurations, as returned by the self.configurations()
     *  computed array, where the configuration is marked as active.
     *  Functions as a filter on the self.configuratons() generated array.
     */
	this.activeConfigurations = ko.computed(function () {
			var out = [];
			for (var i = 0; i < this.configurations().length; i++) {
				if (this.configurations()[i].isActive()) {
					out.push(this.configurations()[i]);
				}
			}
			return out;
		}, this);
    
    /**
     *  inactiveConfigurations
     *
     *  type: computed array
     *
     *  This is a computed function that returns a list of the
     *  configurations, as returned by the self.configurations()
     *  computed array, where the configuration is NOT marked as active.
     *  Functions as a filter on the self.configuratons() generated array.
     */
	this.inactiveConfigurations = ko.computed(function () {
			var out = [];
			for (var i = 0; i < this.configurations().length; i++) {
				if (!this.configurations()[i].isActive()) {
					out.push(this.configurations()[i]);
				}
			}
			return out;
		}, this);
    
    /**
     *  deselectAllConfigurations
     *
     *  type: function
     *
     *  output: array
     *
     *  This takes the list of configurations returned by 
     *  self.configurations() and for each one marked as
     *  selected:
     *    mark as NOT selected
     *    add the deselected entry to the output list
     *  The list returned contains those entries that were
     *  de-selected.
     *  Functions as a filter on the self.configuratons() generated array.
     */
	this.deselectAllConfigurations = function () {
		var out = [];
		for (var i = 0; i < this.configurations().length; i++) {
			if (this.configurations()[i].isSelected()) {
				this.configurations()[i].isSelected(false);
				out.push(this.configurations()[i]);
			}
		}
		return out;
	}
    
	/**
     *  listOfCompareConfigurations
     *
     *  type: observable Array
     *
     *  output: none
     *
     *  This is the list of all configurations selected for comparison; used by the comparerGeneralInfo data lead 
	 *  MJD
     */
	this.listOfCompareConfigurations = ko.observableArray();

	this.showCompareButton = ko.computed(function(){
		var designs = self.listOfCompareConfigurations();
		if(designs.length > 0){
			return true;
		}
		return false;
	},this);
	
	/**
     *  selectedCompareConfigurations
     *
     *  type: function
     *
     *  output: none
     *
     *  This looks for all the configurations selected for comparison, sets their isCompareSelected observable
	 *  accordingly to if its been selected for comparison, hands the list to the listOfCompareConfigurations observable array,
     *  and then forces a rerendering
	 *  MJD
     */
	this.selectedCompareConfigurations = function(id)
	{
		var configsSelected = [];
		$("#" + id + " select").each(function(){
			if ($(this).val() != "")
			{
				configsSelected.push($(this).val());
			}
		});
		
		for (var i = 0; i < this.configurations().length; i++)
		{
			if (jQuery.inArray(this.configurations()[i].design, configsSelected) != -1)
			{
				this.configurations()[i].isCompareSelected(true);
			}
			else
			{
				this.configurations()[i].isCompareSelected(false);
			}
		}
		
		this.listOfCompareConfigurations(configsSelected);
		vf_viz.rerender();
	}
	
	this.dsVisSelected = ko.observable();
	this.dsVisColor = ko.observable();
	
	
	/**
     *  selectedConfigurations
     *
     *  type: observable array
     *
     *  This observable array contains the list of configuration
     *  entries, as returned by self.configurations(), and returns
     *  a list of configuration entries where isSelected is true.
     *  Functions as a filter on the self.configuratons() generated array.
     */
	this.selectedConfigurations = ko.computed(function () {
		var out = [];
		for (var i = 0; i < this.configurations().length; i++) {
			if (this.configurations()[i].isSelected()) {
				out.push(this.configurations()[i]);
			}
		}
		return out;
	}, this);
	
	
	/* 
	*	Creates a list of benches which apply to the currently selected configuration
	*/
	this.configBenches = ko.computed(function (){
		var out = [];
		for(var i = 0; i<self.configurations().length; i++){
			if(self.configurations()[i].design == self.selectedConfiguration()) {
				for(var j = 0; j<self.configurations()[i].testBench.length; j++) {
					out.push(self.configurations()[i].testBench[j].name);
				}
			}
		}
		return out;
	}, this);
	
    /**
     *  unselectedConfigurations
     *
     *  type: observable array
     *
     *  This observable array contains the list of configuration
     *  entries, as returned by self.configurations(), and returns
     *  a list of configuration entries where isSelected is NOT true.
     *  Functions as a filter on the self.configuratons() generated array.
     */
	this.unselectedConfigurations = ko.computed(function () {
		var out = [];
		for (var i = 0; i < this.configurations().length; i++) {
			if (!this.configurations()[i].isSelected()) {
				out.push(this.configurations()[i]);
			}
		}
		return out;
	}, this);
    
	/**
     *  listOfBenches 
     *
     *  type: computed array
     *
     *  This is a computed function that returns a list of the
     *  test benches being used in the configurations without listing duplicates.
     */	
     this.listOfBenches = ko.computed(function () {   
        console.log("vf-dataprovider: listOfBenches: enter");
		var out = [];
		var temp = 0;
		for (var i = 0; i < this.configurations().length; i++) {
			if(isSet('self.configurations()[i].testBench[0].name',{self:this, i:i})){
				if(i==0) {

				out.push(this.configurations()[i].testBench[0].name);
				}
				if(i!=0) {
					temp = this.configurations()[i].testBench[0].name;
					for (var j=0; j<out.length; j++){
						if(out[j]==temp){
							temp = 0;
						}
					}
					if(temp!=0)
					{
						out.push(this.configurations()[i].testBench[0].name);
					}
				}
			}
		}
			return out;
		}, this).extend({
			throttle : 100
		});	
    
	/**
     *  PET 
     *
     *  type: object
     *
     *  This is an object that is keyed to all the test benches.  Each entry
     *  contains an object that is keyed by the metric variable.
     */	
    this.PET = ko.computed(function () {   
			var out = {};
			for(var k = 0; k < this.configurations().length; k++){
				for(var l = 0; l < this.configurations()[k].testBench.length; l++){
					var responses = this.configurations()[k].testBench[l];
					// Add the Response surface data
					if (!out.hasOwnProperty(responses.name) && isSet('responses.PETResults.responses.length', {responses:responses})) {
						var TB = responses.name;
						out[TB] = {};
						for (var i = 0;i < responses.PETResults.responses.length; i++) {
							var metric = responses.PETResults.responses[i].metric;
							if (! out[TB].hasOwnProperty(metric)) {
								out[TB][metric] = responses.PETResults.responses[i];
							}
						}
					}
				}
			}
			return out;
		}, this).extend({
			throttle : 100
		});	

	
	
	this.listOfFidelities = ko.observableArray(["high", "med", "low"]);
	this.selectedFidelity = ko.observable("low");
	/**
     *  petMiniView 
     *
     *  type: object
     *
     *  This is an object that is keyed to all the test benches.  Each entry
     *  contains an object that is keyed by the metric variable.
     */	
    this.petMiniView = ko.observable(false);
    
    /**
     *  listBenchesWithPET
     *
     *  type: observableArray
     *
     *  This will return a array of the Test Benches that appear in the PET object
     */
    this.listOfBenchesWithPET = ko.computed(function () {
			var out = [];
			
			// Enter the test bench into the list
			for (var key in this.PET()) {
				out.push(key);
			}
			return out;
		}, this).extend({
			throttle : 100
		});	
	
	this.selectedBenchWithPET = ko.observable('');
	
    
    /**
     *  listOfPETResponses
     *
     *  type: computed
     *
     *  This will return a array of PET Response objects for the given Test Bench
     */
	 
	function PETLoader(self, pet, tb)
	{
		//self: scope of 
		//pet: pet data for the current testbench selected
		//tb: testbench
		
		var out = [];
		
		for (var key in pet) 
		{
			// console.log("has state ("+key+"): "+self.PET[TB][key].hasOwnProperty('state'))
            // console.log("has state ("+key+"): "+('state' in self.PET[TB][key]))
            
			if (pet[key].hasOwnProperty('state')) 
			{
                // console.log("use saved values");
                var _display = pet[key].state.display;
                var _greaterThan = pet[key].state.greaterThan;
                var _value = pet[key].state.value;
            } 
			else 
			{
                // console.log("use default values");
                var _display = false;
                var _greaterThan = (self.objectives()[tb + "_" + key] > self.thresholds()[tb + "_" + key]) ? 'true' : 'false';
                var _value = 0.5*(pet[key].max + pet[key].min);
            }
			_step = Math.abs(pet[key].max - pet[key].min) / 100;
			
			out.push({
				'display' : ko.observable(_display),
				'name' : ko.observable(key),
				'min' : pet[key].min,
				'max' : pet[key].max,
				'threshold' : self.thresholds()[tb + "_" + key],
				'objective' : self.objectives()[tb + "_" + key],
				'value' : ko.observable(_value),
				'step' : _step,
				'greaterThan' : ko.observable(_greaterThan),
				'rid' : pet[key].rid,
				'color' : ko.observableArray([d3.interpolateHsl("#a912d7","#12d789")(Math.random())])
			});
        }
		
		return out;
	}

    this.listOfPETResponses = ko.computed(function() {
        console.log("vf-dataprovider: listOfPETResponses: enter");
		
        var key;
        //var out = [];
        var entry = {}; //display, name, min, max, value, step, greaterThan, rid
        var TB = self.selectedBenchWithPET();
        var _step;
		
        if (TB === '') return [];
        
		return new PETLoader(self, self.PET()[TB], TB);
		
        /* for (key in self.PET()[TB]) 
		{
            out.push({});
			
			var entry = out[out.length - 1];
			// console.log("has state ("+key+"): "+self.PET[TB][key].hasOwnProperty('state'))
            // console.log("has state ("+key+"): "+('state' in self.PET[TB][key]))
            
			if (self.PET()[TB][key].hasOwnProperty('state')) {
                // console.log("use saved values");
                var _display = self.PET()[TB][key].state.display;
                var _greaterThan = self.PET()[TB][key].state.greaterThan;
                var _value = self.PET()[TB][key].state.value;
            } else {
                // console.log("use default values");
                var _display = false;
                var _greaterThan = 'true';
                var _value = 0.5*(self.PET()[TB][key].max + self.PET()[TB][key].min);
            }
			
            entry.display = ko.observable(_display);
            entry.name = ko.observable(key);
            entry.min = self.PET()[TB][key].min;
            entry.max = self.PET()[TB][key].max;
            entry.value = ko.observable(_value);
			_step = Math.abs(self.PET()[TB][key].max - self.PET()[TB][key].min) / 100;
			entry.valueDisplayed = entry.value;
			entry.step = _step;
			entry.greaterThan = ko.observable(_greaterThan);
			entry.rid = self.PET()[TB][key].rid;
			entry.color = new COLOR(d3.interpolateHsl("#a912d7","#12d789")(Math.random()))
        } */
        return out;
    },this);
	
	this.setAllThresholds = function () {
		for (var i = 0; i < this.listOfPETResponses().length; i++)
		{
			if (typeof this.listOfPETResponses()[i].threshold != 'undefined')
			{
				this.listOfPETResponses()[i].value(this.listOfPETResponses()[i].threshold);
			}
		}
	},
	
	this.setAllObjectives = function () {
		for (var i = 0; i < this.listOfPETResponses().length; i++)
		{
			if (typeof this.listOfPETResponses()[i].objective != 'undefined')
			{
				this.listOfPETResponses()[i].value(this.listOfPETResponses()[i].objective);
			}
		}
	},
	
    this.setAllPETResponses = function() {
        console.log("vf-dataprovider: setAllPETResponses: enter");
        var i;
        var TB = self.selectedBenchWithPET();
        
        if (TB === '' || TB === undefined) return out;
        
        for (i=0; i<self.listOfPETResponses().length; i++) {
            console.log("vf-dataprovider: setAllPETResponses: before: "+self.listOfPETResponses()[i].display());
            self.listOfPETResponses()[i].display(true);
            console.log("vf-dataprovider: setAllPETResponses: after: "+self.listOfPETResponses()[i].display());
        }
    }
    this.clearAllPETResponses = function() {
        console.log("vf-dataprovider: clearAllPETResponses: enter");
        var i;
        var TB = self.selectedBenchWithPET();
        
        if (TB === '') return out;
        
        for (i=0; i<self.listOfPETResponses().length; i++) {
            self.listOfPETResponses()[i].display(false);
        }
    }
    
    /**
     *  listOfPETVariables
     *
     *  type: computed
     *
     *  This will return a array of PET Input (Variable) objects for the given Test Bench
     */
    this.listOfPETVariables = ko.computed(function() {
        console.log("vf-dataprovider: listOfPETVariables: enter");
    
        var i;
        var key;
        var out = [];
        var entry = {}; //display, name, min, max, value
        var TB = self.selectedBenchWithPET();
        var responses = [];
        var v;
        
        if (TB === '' || TB === undefined) return out;
        
        for (key in self.PET()[TB]) {
            responses.push(key);
        }
        v = responses[0];
        
        if(isSet("self.PET()[TB][v].inputs.length", {self:self, TB:TB, v:v})){
	        for (i = 0; i < self.PET()[TB][v].inputs.length; i++) {
	            entry = {};
	            if (self.PET()[TB][v].inputs[i].hasOwnProperty('state')) {
	                var _display = self.PET()[TB][v].inputs[i].state.display;
	                var _value = self.PET()[TB][v].inputs[i].state.value;
	            } else {
	                var _display = false;
	                var _value = 0.5*(self.PET()[TB][v].inputs[i].max + self.PET()[TB][v].inputs[i].min);
	            }
	            entry.display = ko.observable(_display);
	            entry.name = ko.observable(self.PET()[TB][v].inputs[i].label);
	            entry.min = self.PET()[TB][v].inputs[i].min;
	            entry.max = self.PET()[TB][v].inputs[i].max;

	            entry.value = ko.observable(_value);
	            entry.step = Math.abs(self.PET()[TB][v].inputs[i].max - self.PET()[TB][v].inputs[i].min)/100;
	            entry.greaterThan = ko.observable("true");
	            out.push(entry);
	        }
	    }
        return out;
    });
    this.setAllPETVariables = function() {
        console.log("vf-dataprovider: setAllPETVariables: enter");
        var i;
        var TB = self.selectedBenchWithPET();
        
        if (TB === '') return out;
        
        for (i=0; i<self.listOfPETVariables().length; i++) {
            self.listOfPETVariables()[i].display(true);
        }
    }
    this.clearAllPETVariables = function() {
        console.log("vf-dataprovider: clearAllPETVariables: enter");
        var i;
        var TB = self.selectedBenchWithPET();
        
        if (TB === '') return out;
        
        for (i=0; i<self.listOfPETVariables().length; i++) {
            self.listOfPETVariables()[i].display(false);
        }
    }
	
	/**
     *  showPetWidgets 
     *
     *  type: boolean
     *
     *  This is an object that is keyed to all the test benches.  Each entry
     *  contains an object that is keyed by the metric variable.
     */	
    this.showPetWidgets = ko.computed(function () {

			var numOfResponses = 0;
			var numOfvariables = 0;
			
			for(var i = 0; i < this.listOfPETResponses().length; i++){
				if(this.listOfPETResponses()[i].display()){
					numOfResponses++;
				}
			}
			
			for(var i = 0; i < this.listOfPETVariables().length; i++){
				if(this.listOfPETVariables()[i].display()){
					numOfvariables++;
				}
			}
			//miyako - show even responses not selected
			if ( numOfvariables >= 2)
				return true;
			//end miyako	
			if(numOfResponses > 0 && numOfvariables >= 2){
				return true;
			}
			
			return false;
		}, this);	

   	/**
     *  showPet 
     *
     *  type: boolean
     *
     *  This is an object that is keyed to all the test benches.  Each entry
     *  contains an object that is keyed by the metric variable.
     */	
    this.showPet = ko.computed(function () {
			
			if(self.showExperimental() || Object.listKeys(self.PET()).length > 0){
				return true;
			}
			
			return false;
		}, this);	

    /**
     *  selectConfigurationsById
     *
     *  type: function
     *
     *  inpute: id - identifies the configuration to select, the design name
     *  output: array
     *
     *  Look through the deselected configurations (self.deselectAllConfigurations())
     *  and set isSelected on the one that matches the given id parameter.
     */
	this.selectConfigurationById = function (id) {
		this.deselectAllConfigurations();
		for (var i = 0; i < this.configurations().length; i++) {
			if (this.configurations()[i].design == id) {
				this.configurations()[i].isSelected(true);
			}
		};
	}
    
    /**
     *  getConfigurationsNameById
     *
     *  type: function
     *
     *  inpute: id - identifies the configuration to select, the design name
     *  output: string
     *
     *  Look through the all configurations (self.deselectAllConfigurations())
     *  and return the name of the design that matches the given id parameter.
     */
	this.getConfigurationsNameById = function (id) {
		for (var i = 0; i < this.configurations().length; i++) {
			if (this.configurations()[i].designId == id) {
				return this.configurations()[i].design;
			}
		};
		return id;
	}
	
    /**
     *  getConfigurationByName
     *
     *  type: function
     *
     *  inpute: design name - identifies the configuration to select, the design name
     *  output: object
     *
     *  Look through the each configuration
     *  and return the configuration of the design that matches the given name parameter.
     */
	this.getConfigurationByName = function (name) {
		for (var i = 0; i < this.configurations().length; i++) {
			if (this.configurations()[i].design === name) {
				return this.configurations()[i];
			}
		};
		return null;
	}
	
    /**
     *  allVariables
     *
     *  type: computed array
     *
     *  This is a computed function that returns a list of the
     *  variables.  The list of variables is culled from the configurations
     *  as returned by the self.configurations() computed array.
     */
	this.allVariables = ko.computed(function () {
			var variables = [];
            
            // Loop over configurations and append its variables to this new list
			for (var i = 0; i < this.configurations().length; i++) {
				for (var j = 0; j < this.configurations()[i].variables.length; j++) {
					if(this.configurations()[i].variables[j].type !== "limit"){
						this.configurations()[i].variables[j].designs = [this.configurations()[i].design];
						variables.push(this.configurations()[i].variables[j]);
					}
				}
			}
            
            // Remove the duplicate variable entries
			for (var i = 0; i < variables.length; i++) {
				for (var j = 0; j < variables.length; j++) {
					if (variables[i].Name == variables[j].Name && variables[i].testBench == variables[j].testBench && i != j) {
						for (var k = 0; k < variables[j].designs.length; k++) {
							if(variables[i].designs.inArray(variables[j].designs[k]) === false){
								variables[i].designs.push(variables[j].designs[k]);
							}
						}
						variables.splice(j, 1);
						i = 0;
						j = 0;
					}
				}
			}
			
			// sort array
			variables.sort(function(a,b){
				if([a.Name,b.Name].sort().indexOf(a.Name)==1){
					return 1;
				}
				else{
					return -1;
				}
			});
			
			// convert all true or false to 1 and 0
			// for (var i = 0; i < variables.length; i++) {
				// if(){
					// variables[i].value = 1;
				// }
				// if(){
					// variables[i].value = 0;
				// }
			// }
			
			return variables
		}, this);
		
    
	/**
     *  numberOfVariables
     *
     *  type: computed
     *
     *  output: Number of Variables
     *
     */
	this.numberOfVariables = ko.computed(function () {
			return this.allVariables().length;
		}, this);
	
    /**
     *  commonVariables
     *
     *  type: computed array
     *
     *  Given a list of active configurations, return a list of variable
     *  entries that appear in every one of the active configurations, the
     *  common set.
     *
     *  References:  naba_utils.js:  Array.prototype.indexOfAll:  function
     */
	this.commonVariables = ko.computed(function () {
    
            // Make a reference copy of the self.allVariables() computed array
			var variables = this.allVariables();
            
            // Create and initialize a ?? array
			var stats = [];
			for (var i = 0; i < variables.length; i++) {
				stats.push(0);
			}
            
            /*
                Loop over the set all all active configurations and mark the variables that are used
                in those configurations.  This is kept as a non-zero count in the stats array, which
                is indexed by the variable.
             */
            // Loop over all configurations
			for (var i = 0; i < this.configurations().length; i++) {
            
                // Process only configuration where isActive is true
				if (this.configurations()[i].isActive()) {
                
                    // Loop over the variables associated with the configuration - j
					for (var j = 0; j < this.configurations()[i].variables.length; j++) {
                    
                        // Loop over all the known variables - k
						for (var k = 0; k < variables.length; k++) {
                        
                            // If the configuration name and testbench  match the variables name and testbench then increment the stat for this variable (k)
							if (this.configurations()[i].variables[j].Name == variables[k].Name && this.configurations()[i].variables[j].testBench == variables[k].testBench) {
								stats[k]++;
							}
						}
					}
				}
			}
            
            // Build the output array
			if (this.activeConfigurations().length != 0) {
            
                // If there are ANY active configurations
                // Get a list of indices where the variable is referenced by all of the active configuration
				var common = stats.indexOfAll(this.activeConfigurations().length);
				var out = [];
                
                // Return list of variable entries that were found by above code
				for (var i = 0; i < common.length; i++) {
					out.push(variables[common[i]]);
				}
				return out;
			} else {
            
                // There are NO active configurations, return an empty list
				return [];
			}
		}, this);
    
    /**
     *  uncommonVariables
     *
     *  type: computed array
     *
     *  Given a list of common variables from self.commonVariables, generate a list 
     *  of the remaining variables.
     */
	this.uncommonVariables = ko.computed(function () {
			var variables = this.allVariables();
			var common = this.commonVariables();
			var out = [];
			for (var i = 0; i < variables.length; i++) {
				if (common.indexOf(variables[i]) == -1) {
					out.push(variables[i]);
				}
			}
			return out;
		}, this);
    
    /**
     *  variables
     *
     *  type: computed array
     *
     *  Return a list of all the variables, generated from the self.allVariables()
     *  computed function.  The output list has an isActive observable added that
     *  is initially set to false.
     */
	this.variables = ko.computed(function () {
			var allVariables = this.allVariables();
			var variables = [];
            var _isTopsis;
            var _isActive;
            var _weighting;
            var _optimize;
            var i;
            var j;
            
            // Loop over the full list of variables, initially set active to false
			for (var i = 0; i < allVariables.length; i++) {
            
                // The default state
                _isActive = false;
                _isTopsis = false;
                _weighting = 50;
                _optimize = 1;

                // Set defaults from state
                if (state !== null) {
                    console.log("RESTORE:  "+allVariables[i].Name+":"+allVariables[i].testBench);
                    
                    // Find activeVariables
                    for (j=0; j<state.activeVariables.length; j++) {
                        console.log(" a     :  "+state.activeVariables[j].Name+":"+state.activeVariables[j].testBench);
                        if (state.activeVariables[j].Name === allVariables[i].Name && state.activeVariables[j].testBench === allVariables[i].testBench) {
                            console.log("active:  "+state.activeVariables[j].Name+":"+state.activeVariables[j].testBench);
                            _isActive = state.activeVariables[j].isActive;
                            break;
                        }
                    }
                    console.log(_isActive);
                
                    // Find topsisVariables
                    for (j=0; j<state.topsisVariables.length; j++) {
                        console.log(" t     :  "+state.topsisVariables[j].Name+":"+state.topsisVariables[j].testBench);
                        if (state.topsisVariables[j].Name === allVariables[i].Name && state.topsisVariables[j].testBench === allVariables[i].testBench) {
                            console.log("topsis:  "+state.topsisVariables[j].Name+":"+state.topsisVariables[j].testBench);
                            _isTopsis = state.topsisVariables[j].isTopsis;
                            _weighting = state.topsisVariables[j].weighting;
                            _optimize = state.topsisVariables[j].optimize;
                            break;
                        }
                    }
                    console.log(_isTopsis);
                }
                
                // Set the values
                variables.push({
                    Name : allVariables[i].Name,
                    designs : allVariables[i].designs,
                    isActive : ko.observable(_isActive),
                    isTopsis : ko.observable(_isTopsis),
                    testBench : allVariables[i].testBench,
                    Unit : allVariables[i].Unit,
                    type : allVariables[i].type,
                    weighting : ko.observable(_weighting),
                    optimize : ko.observable(_optimize),
                });

            }

			return variables;
		}, this);
   
    /**
     *  unselectAllConfigurations
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.configurations() computed array
     *  and set isActive to false.
     */
	this.unselectAllConfigurations = function () {
			for (var i = 0; i < this.configurations().length; i++) {
				this.configurations()[i].isActive(false);
			}
		}
   
   
    /**
     *  selectAllConfigurations
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.configurations() computed array
     *  and set isActive to true.
     */
	this.selectAllConfigurations = function () {
			for (var i = 0; i < this.configurations().length; i++) {
				this.configurations()[i].isActive(true);
			}
		}
    /**
     *  unselectAllVariables
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isActive to false.
     */
	this.unselectAllVariables = function () {
			for (var i = 0; i < this.variables().length; i++) {
				this.variables()[i].isActive(false);
			}
		}
   
    /**
     *  selectAllVariables
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isActive to true.
     */
	this.selectAllVariables = function () {
			for (var i = 0; i < this.variables().length; i++) {
				this.variables()[i].isActive(true);
			}
		}
		
			/**
	* selectedBench 
	*
	*/
	this.selectedBench = ko.observable(this.listOfBenches()[0]);
	
   
    /**
     *  unselectAllMetrics
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isActive to false for all variables that are of
     *  type metric.
     */
	this.unselectAllMetrics = function () {
			for (var i = 0; i < this.variables().length; i++) {
				if(this.variables()[i].type == "metric"){
					this.variables()[i].isActive(false);
				}
			}
		}
   
    /**
     *  selectAllMetrics
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isActive to true for all variables that are of
     *  type metric.
     */
	this.selectAllMetrics = function () {
			for (var i = 0; i < this.variables().length; i++) {
				if(this.variables()[i].type == "metric"){
					this.variables()[i].isActive(true);
				}
			}
		}
   
    /**
     *  unselectAllParameters
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isActive to false for all variables that are of
     *  type parameter.
     */
	this.unselectAllParameters = function () {
			for (var i = 0; i < this.variables().length; i++) {
				if(this.variables()[i].type == "parameter"){
					this.variables()[i].isActive(false);
				}
			}
		}
   
    /**
     *  selectAllParameters
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isActive to true for all variables that are of
     *  type parameter.
     */
	this.selectAllParameters = function () {
			for (var i = 0; i < this.variables().length; i++) {
				if(this.variables()[i].type == "parameter"){
					this.variables()[i].isActive(true);
				}
			}
		}
    
    /**
     *  activeVariables
     *
     *  type: computed array
     *
     *  Return a list of all the variables, generated from the self.allVariables()
     *  computed function, that are active.
     */
	this.activeVariables = ko.computed(function () {
			var variables = this.variables();
			var out = [];
			for (var i = 0; i < variables.length; i++) {
				if (variables[i].isActive()) {
					out.push(variables[i]);
				};
			}
			return out;
		}, this);
   
    /**
     *  inactiveVariables
     *
     *  type: computed array
     *
     *  Return a list of all the variables, generated from the self.allVariables()
     *  computed function, that are NOT active.
     */
	this.inactiveVariables = ko.computed(function () {
			var variables = this.variables();
			var out = [];
			for (var i = 0; i < variables.length; i++) {
				if (!variables[i].isActive()) {
					out.push(variables[i]);
				};
			}
			return out;
		}, this);

	/**
     *  isVariableActive
     *
     *  type: function
     *
     *  input: testBench(String), name(String)
     *  output: none
     *
     *  Loop through all entries in self.activeVariables() computed array
     *  and check if the given variable within the given test bench is active.
     */
	this.isVariableActive = function(testBench, name){
		for(var i = 0; i < this.activeVariables().length; i++){
			if(testBench == this.activeVariables()[i].testBench && name == this.activeVariables()[i].Name){
				return true;
			}
		}
		return false;
	}
	
	
	/**
     *  getVariableWeighting
     *
     *  type: function
     *
     *  input: testBench(String), name(String)
     *  output: none
     *
     *  Loop through all entries in self.topsisVariables() computed array
     *  and check if the given variable within the given test bench is active.
     */
	this.getVariableWeighting = function(testBench, name){
		for(var i = 0; i < this.topsisVariables().length; i++){
			if(testBench == this.topsisVariables()[i].testBench && name == this.topsisVariables()[i].Name){
				return true;
			}
		}
		return false;
	}
	
	/**
     *  getVariableOptimize
     *
     *  type: function
     *
     *  input: testBench(String), name(String)
     *  output: none
     *
     *  Loop through all entries in self.topsisVariables() computed array
     *  and check if the given variable within the given test bench is active.
     */
	this.getVariableOptimize = function(testBench, name){
		for(var i = 0; i < this.topsisVariables().length; i++){
			if(testBench == this.topsisVariables()[i].testBench && name == this.topsisVariables()[i].Name){
				return true;
			}
		}
		return false;
	}
   
    /**
     *  unselectAllVariablesForTopsis
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isTopsis to true.
     */
	this.unselectAllVariablesForTopsis = function () {
			for (var i = 0; i < this.variables().length; i++) {
				this.variables()[i].isTopsis(false);
			}
		}
   
    /**
     *  selectAllVariablesForTopsis
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isTopsis to true.
     */
	this.selectAllVariablesForTopsis = function () {
			for (var i = 0; i < this.variables().length; i++) {
				this.variables()[i].isTopsis(true);
			}
		}
   
    /**
     *  unselectAllMetricsForTopsis
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isTopsis to false for all variables that are of
     *  type metric.
     */
	this.unselectAllMetricsForTopsis = function () {
			for (var i = 0; i < this.variables().length; i++) {
				if(this.variables()[i].type == "metric"){
					this.variables()[i].isTopsis(false);
				}
			}
		}
   
    /**
     *  selectAllMetricsForTopsis
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isTopsis to true for all variables that are of
     *  type metric.
     */
	this.selectAllMetricsForTopsis = function () {
			for (var i = 0; i < this.variables().length; i++) {
				if(this.variables()[i].type == "metric"){
					this.variables()[i].isTopsis(true);
				}
			}
		}
   
    /**
     *  unselectAllParametersForTopsis
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isTopsis to false for all variables that are of
     *  type parameter.
     */
	this.unselectAllParametersForTopsis = function () {
			for (var i = 0; i < this.variables().length; i++) {
				if(this.variables()[i].type == "parameter"){
					this.variables()[i].isTopsis(false);
				}
			}
		}
   
    /**
     *  isRequirement
     *
     *  type: function
     *
     *  input: name - the name of the variable to check for
     *  output: boolean - true: name appears in the requirements list
     *
     *  Loop through all entries in self.requirements() computed array
     *  and check if the name is present in the requirements array.  If
     *  return true else false.
     */
    this.isRequirement = function(name, testBench) {
        for (var i = 0; i < this.requirements().length; i++) {
            if (this.requirements()[i].name === name && this.requirements()[i].testBench === testBench) return true;
        }
        return false;
    }
    
    /**
     *  selectAllParametersForTopsis
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isTopsis to true for all variables that are of
     *  type parameter.
     */
	this.selectAllParametersForTopsis = function () {
			for (var i = 0; i < this.variables().length; i++) {
				if(this.variables()[i].type == "parameter"){
					this.variables()[i].isTopsis(true);
				}
			}
		}
   
    /**
     *  unselectAllRequirementsForTopsis
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isTopsis to false for all variables that are of
     *  type parameter.
     */
	this.unselectAllRequirementsForTopsis = function () {
			for (var i = 0; i < this.variables().length; i++) {
				if(this.isRequirement(this.variables()[i].Name)){
					this.variables()[i].isTopsis(false);
				}
			}
		}
   
    /**
     *  selectAllRequirementsForTopsis
     *
     *  type: function
     *
     *  input: none
     *  output: none
     *
     *  Loop through all entries in self.variables() computed array
     *  and set isTopsis to true for all variables that are of
     *  type parameter.
     */
	this.selectAllRequirementsForTopsis = function () {
			for (var i = 0; i < this.variables().length; i++) {
				if(this.isRequirement(this.variables()[i].Name, this.variables()[i].testBench)){
					this.variables()[i].isTopsis(true);
				}
			}
		}
     
    /**
     *  topsisVariables
     *
     *  type: computed array
     *
     *  Return a list of all the variables, generated from the self.allVariables()
     *  computed function, that are selected for topsis.
     */
	this.topsisVariables = ko.computed(function () {
			var variables = this.variables();
			var out = [];
			for (var i = 0; i < variables.length; i++) {
				if (variables[i].isTopsis()) {
					out.push(variables[i]);
				};
			}
            // add requirements
			return out;
		}, this);
   
    /**
     *  untopsisVariables
     *
     *  type: computed array
     *
     *  Return a list of all the variables, generated from the self.allVariables()
     *  computed function, that are NOT selected for topsis.
     */
	this.untopsisVariables = ko.computed(function () {
			var variables = this.variables();
			var out = [];
			for (var i = 0; i < variables.length; i++) {
				if (!variables[i].isTopsis()) {
					out.push(variables[i]);
				};
			}
            // add requirements
			return out;
		}, this);
  
    // functions to:  select and unselect topsis variables, metrics, parameters, requirements
	
		/**
     *  isVariableTopsis
     *
     *  type: function
     *
     *  input: testBench(String), name(String)
     *  output: none
     *
     *  Loop through all entries in self.topsisVariables() computed array
     *  and check if the given variable within the given test bench is active.
     */
	this.isVariableTopsis = function(testBench, name){
		for(var i = 0; i < this.topsisVariables().length; i++){
			if(testBench == this.topsisVariables()[i].testBench && name == this.topsisVariables()[i].Name){
				return true;
			}
		}
		return false;
	}
  
    /**
     *  commonConfigurations
     *
     *  type: computed array
     *
     *  Given a list of active variables, return a list of configuration
     *  entries that appear in every one of the active variables, the
     *  common set.
     *
     *  References:  naba_utils.js:  Array.prototype.indexOfAll:  function
     */

	this.commonConfigurations = ko.computed(function () {
			var configurations = this.configurations();
			var variables = this.activeVariables();
            
            // Initialize the stats array
			var stats = [];
			for (var i = 0; i < configurations.length; i++) {
				stats.push(0);
			}
            
            // Loop over all variables
			for (var i = 0; i < variables.length; i++) {
            
                // Loop over only active variables
				if (variables[i].isActive()) {
                
                    // Loop over the list of configurations - j
					for (var j = 0; j < configurations.length; j++) {
                    
                        // Loop over the variables that are associated with this configuration - k
						for (var k = 0; k < configurations[j].variables.length; k++) {
                        
                            // If the configuration variable name and testbench  match the variables name and testbench then increment the stat for this variable (j)
							if (configurations[j].variables[k].Name == variables[i].Name && configurations[j].variables[k].testBench == variables[i].testBench) {
								stats[j]++;
							}
						}
					}
				}
			}
            
            // Build the output array
			if (this.activeVariables().length != 0) {
            
                // If there are ANY active variables
                // Get a list of indices where the variable is referenced by all of the active variables
				var common = stats.indexOfAll(this.activeVariables().length);
				var out = [];
                
                // Return list of configuration entries that were found by above code
				for (var i = 0; i < common.length; i++) {
					out.push(configurations[common[i]]);
				}
				return out;
			} else {
            
                // There are NO active varables, return an empty list
				return [];
			}
		}, this);
   
    /**
     *  uncommonConfigurations
     *
     *  type: computed array
     *
     *  Given a list of configurations, and the list of common configurations,
     *  return a list of the remaining configurations.
     */
    this.uncommonConfigurations = ko.computed(function () {
        var configurations = this.configurations();
		var common = this.commonConfigurations();
		var out = [];
		for (var i = 0; i < configurations.length; i++) {
			if (common.indexOf(configurations[i]) == -1) {
				out.push(configurations[i]);
			}
		}
		return out;
	}, this);
   
    /**
     *  commonActiveVariables
     *
     *  type: computed array
     *
     *  Given a list of variables common among a list of configurations,
     *  return a list of the variables which are common and active.
     */
    this.commonActiveVariables = ko.computed(function () {
        var common = this.commonVariables();
		var active = this.activeVariables();
		var out = [];
		for (var i = 0; i < active.length; i++) {
			for(var j = 0; j < common.length; j++){
				if (active[i].Name == common[j].Name && active[i].testBench == common[j].testBench) {
					out.push(active[i]);
				}
			}
		}
		return out;
	}, this);
	
	 /**
     *  commonActiveConfigurations
     *
     *  type: computed array
     *
     *  Given a list of configurations which are associated with a list of active variables,
     *  return a list of the configurations which are common and active.
     */
    this.commonActiveConfigurations = ko.computed(function () {
        var common = this.commonConfigurations();
		var active = this.activeConfigurations();
		var out = [];
		for (var i = 0; i < active.length; i++) {
			for(var j = 0; j < common.length; j++){
				if (active[i].id == common[j].id) {
					out.push(active[i]);
				}
			}
		}
		return out;
	}, this);
	
	/**
     *  commonInactiveConfigurations
     *
     *  type: computed array
     *
     *  Given a list of configurations which are associated with a list of active variables,
     *  return a list of the configurations which are not common and active.
     */
    this.commonInactiveConfigurations = ko.computed(function () {
        var common = this.commonConfigurations();
		var out = [];
		for (var i = 0; i < common.length; i++) {
			if (common[i].isActive() == false) {
				out.push(common[i]);
			}
		}
		return out;
	}, this);
	
	/**
     *  commonInactiveVariables
     *
     *  type: computed array
     *
     *  Given a list of configurations, and the list of common configurations,
     *  return a list of the remaining configurations.
     */
    this.commonInactiveVariables = ko.computed(function () {
        var active = this.activeVariables();
		var common = this.commonVariables();
		var out = [];
		var found = false;
		for (var i = 0; i < common.length; i++) {
			for(var j = 0; j < active.length; j++){
				if (common[i].Name == active[j].Name && common[i].testBench == active[j].testBench) {
					found = true;
				}
			}
			if(found == false){
				out.push(common[i]);
			}
			else{
				found = false;
			}
		}
		return out;
	}, this);
	
    /**
     *  byVariable
     *
     *  type: computed boolean
     *
     *  This is a reference observable used to indicate whether to operate
     *  by variable or by configuration.
     */
	this.byVariable = ko.observable(true);
	
	/**
     *  dsaTab
     *
     *  type: computed boolean
     *
     *  This is a reference observable used to indicate whether to show
     *  the variables tab or the configurations tab within the setting modal.
	 *  If true show variables; if false show configurations
     */
	this.dsaTab = ko.observable(true);
   
    /**
     *  currentConfigurations
     *
     *  type: computed array
     *
     *  Returns a list of current configurations depending on current value
     *  of self.byVariable
     *
     *  byVariable = true: return self.commonConfigurations()
     *  byVariable = false: return self.activeConfigurations()
     */
	this.currentConfigurations = ko.computed(function () {
			if (this.byVariable()) {
				return this.commonActiveConfigurations();
			} else {
				return this.activeConfigurations();
			}
		}, this);
   
    /**
     *  uncurrentConfigurations
     *
     *  type: computed array
     *
     *  Returns a list of non-current configurations depending on current value
     *  of self.byVariable
     *
     *  byVariable = true: return self.uncommonConfigurations()
     *  byVariable = false: return self.inactiveConfigurations()
     */
	this.uncurrentConfigurations = ko.computed(function () {
			if (this.byVariable()) {
				return this.commonInactiveConfigurations();
			} else {
				return this.inactiveConfigurations();
			}
		}, this);
   
    /**
     *  currentVariables
     *
     *  type: computed array
     *
     *  Returns a list of current configurations depending on current value
     *  of self.byVariable
     *
     *  byVariable = true: return self.activeVariables()
     *  byVariable = false: return self.commonVariables()
     */
	this.currentVariables = ko.computed(function () {
			if (this.byVariable()) {
				return this.activeVariables();
			} else {
				return this.commonActiveVariables();
			}
		}, this);
   
    /**
     *  uncurrentVariables
     *
     *  type: computed array
     *
     *  Returns a list of non-current configurations depending on current value
     *  of self.byVariable
     *
     *  byVariable = true: return self.inactiveVariables()
     *  byVariable = false: return self.uncommonVariables()
     */
	this.uncurrentVariables = ko.computed(function () {
			if (this.byVariable()) {
				return this.inactiveVariables();
			} else {
				return this.commonInactiveVariables();
			}
		}, this);
  
    /**
     *  scores.selfCertified
     *
     *  type: computed array
     *
     *  Returns an emtpy list
     */
	// this.scores.selfCertified = ko.computed(function () {
		// var out = [];
	// }, this);
  
    /**
     *  activeVariablesNotInConfiguration
     *
     *  type: function
     *
     *  input: configuration object - the single configuration object
     *  output: array of variable names
     *
     *  This function takes a configuration object scans its variable list
     *  and returns a list of names from the active list that it does not
     *  have.
     */
     this.activeVariablesNotInConfiguration = function(configEntry) {
        var out = [];
        
        /**
         *  presentInConfiguration
         *
         *  type: function
         *
         *  input: name - name of variable to check
         *  output: boolean - true if the supplied variable is in the
         *                    configurations list
         *
         *  Given a variable name, determine whether it is present
         *  in the configuration's variable list.
         */
        var presentInConfiguration = function(name) {
            for (var i=0; i<configEntry.variables.length; i++) {
                if (configEntry.variables[i].Name === name) return true;
            }
            return false;
        }
        
        // Loop over all variables
        for (var i=0; i<this.variables().length; i++) {
     
            // Process only active variables
            if (this.variables()[i].isActive && !presentInConfiguration(this.variables()[i].Name)) out.push(this.variables()[i].Name);
        }
        
        return out;
     }
 
    /**
     *  toggleActive
     *
     *  type: function
     *
     *  input: item - takes an entry and toggles it's isActive flag
     *  output: none
     *
     *  This function takes an entry (variable or configuration) and toggles
     *  it's isActive flag.
     */
	this.toggleActive = function(item){
		if(item.isActive()){
			item.isActive(false);
		}
		else{
			item.isActive(true);
		}
	}	
  
    /**
     *  toggleTopsis
     *
     *  type: function
     *
     *  input: item - takes an entry and toggles it's isTopsis flag
     *  output: none
     *
     *  This function takes an entry (variable or configuration) and toggles
     *  it's isTopsis flag.
     */
	this.toggleTopsis = function(item){
		if(item.isTopsis()){
			item.isTopsis(false);
		}
		else{
			item.isTopsis(true);
		}
	}	
	
	/**
     *  allRequirementsFromVF
     *
     *  type: observable
     */
	this.allRequirementsFromVF = ko.observable({});
	
	/**
     *  allRequirementsIds
     *
     *  type: observable
     */
	this.allRequirementsIds = ko.computed(function () {
		return Object.keys(this.allRequirementsFromVF());
	}, this);
	
	/**
     *  currentRequirementsId
     *
     *  type: observable
     */
	this.currentRequirementsId = ko.observable('');
	
    /**
     *  requirementsStructure
     *
     *  type: computed object
     *
     *  This is where the requirements data is stored.
     */
	this.requirementsStructure = ko.computed(function () {
		if(self.currentRequirementsId() === '' && Object.listKeys(self.allRequirementsFromVF()).length > 0){
			self.currentRequirementsId(Object.listKeys(self.allRequirementsFromVF())[0]);
		}
		if(isSet('self.allRequirementsFromVF()[self.currentRequirementsId()]',{self:self})){
			this.getScores();
			return this.allRequirementsFromVF()[this.currentRequirementsId()];
		}
		return {};
	}, this);
   
    /**
     *  requirements
     *
     *  type: computed array
     *
     *  The contents of this array are the requirements, relative to the
     *  current configuration.
     */
	this.requirements = ko.computed(function () {
			var base = this.requirementsStructure();
			var out = [];
			
			/**
             *  searchInObject
             *
             *  type:  function
             *
             *  input: x - recursive search ??
             *
             *  This function loads the out array.  It converts the hierarchical self.requirementsSturcture()
             *  to a linear array (out) of objects.
             */
			var searchInObject = function (x) {
				if (Array.isArray(x)) {
					for (var i = 0; i < x.length; i++) {
						searchInObject(x[i]);
					}
				}
				if (typeof x['children'] != 'undefined') {
					searchInObject(x['children']);
				}
				if ((typeof x['children'] == 'undefined') && (!Array.isArray(x))) {
					out.push(x);
				}
			}
            
            // Search the self.requirementsStructure() observable array, load out array
			searchInObject(base);
            console.log("vf_dataprovider.js:  requirements:  out-start");
            console.log(out);
            console.log("vf_dataprovider.js:  requirements:  out-end");
			
			/*
                Insert currently selected design values as currentValue within the requirements structure
                Find currently selected configuration
             */
            // Get the selected configuration (not a list)
			var selected = this.selectedConfiguration();
            
            // Get the list of all configurations (name only)
			var list = this.listOfConfigurations();
			
			// If configurations are ready (length of list > 0)
			if(list.length > 0 && typeof selected != 'undefined'){
            
                // Locate the selected configuration in the list - this sets the value of the index i
				for(var i = 0; i < list.length; i++){
					if(list[i] == selected){
						break;
					}
				}
                
                // Get the ith (selected) configuration
				var config = this.configurations()[i];
                
                // Loop over the requirements array
				for(var j = 0; j < out.length; j++){
                
                    // Set the currentVal to null
					out[j].currentVal = null;
					out[j].TierLevel = 0;
                    
                    // Loop over the variables in the configuration
					for(var k = 0; k < config.variables.length; k++){
                    
						// Set a new currentVal if the test bench matches and the configurations's variable matches (parameters have an appended P)
						if(config.variables[k].rid && out[j].rid && config.variables[k].rid === out[j].rid && config.variables[k].TierLevel && config.variables[k].TierLevel > out[j].TierLevel){
							out[j].currentVal = +config.variables[k].Value;
						}
						else{
							if(config.variables[k].testBench == out[j].testBench && (config.variables[k].Name == out[j].metricName || config.variables[k].Name == (out[j].metricName + ' (P)'))){
								out[j].currentVal = +config.variables[k].Value;
							}
						}
					}
				}
			}
			
            // Return the list of requirements with, possibly, updated values
			return out;
		}, this).extend({
			throttle : 100
		});
		
	/**
     *  requirementCategories
     *
     *  type: computed array
     *
     *  The contents of this array are the requirements, relative to the
     *  current configuration.
     */
	this.requirementCategories = ko.computed(function () {
		var base = this.requirementsStructure();
		var out = [];
		var searchInObject = function (x) {
			if (Array.isArray(x)) {
				for (var i = 0; i < x.length; i++) {
					searchInObject(x[i]);
				}
			}
			if (typeof x['children'] != 'undefined' && (!Array.isArray(x))) {
				if(typeof x['category'] != 'undefined'){
					if(x['category']){
						out.push(x.name);
					}
				}
				searchInObject(x['children']);
			}
		}
		
		// Search the self.requirementsStructure() observable array, load out array
		searchInObject(base);
		
		//temp hard code categories
		//out = ['MAUF','Automotive Performance','Aggregated Score'];
		
		// reverse order
		out.reverse();
		
		return out;		
	}, this).extend({
			throttle : 100
	});
	
	/**
     *  showRequirementTab
     *
     *  type: computed array
     */
	this.showRequirementTab = ko.computed(function () {
		var req = this.requirementsStructure()
		if((typeof req.name === 'string' && req.name !== 'Undefined') || self.showExperimental()){
			return true;
		}
		return false;
	}, this).extend({
			throttle : 100
	});
	
	/**
     *  calculateRequirementsAnalysis
     *
     *  type: function
     */
	this.calculateRequirementsAnalysis = function () {
		// If requirements data is present calculate overall standing based on objective and threshold
		if(this.showRequirementTab()){
			for(var i = 0; i < this.configurations().length; i++){
				var red = false;
				var gray = false;
				var green = false;
				var blue = false;
				var found = false;
				for(var j = 0; j < this.requirements().length; j++){
					if(red){
						break;
					}
					found = false;
					for(var k = 0; k < this.configurations()[i].variables.length; k++){
						if(this.requirements()[j].testBench === this.configurations()[i].variables[k].testBench && this.requirements()[j].metricName === this.configurations()[i].variables[k].Name){
							found = true;
							if((+this.configurations()[i].variables[k].Value < this.requirements()[j].threshold && this.requirements()[j].threshold < this.requirements()[j].objective) || (+this.configurations()[i].variables[k].Value > this.requirements()[j].threshold && this.requirements()[j].threshold > this.requirements()[j].objective)){
								red = true;
								break;
							}
							if((+this.configurations()[i].variables[k].Value < this.requirements()[j].objective && +this.configurations()[i].variables[k].Value > this.requirements()[j].threshold && this.requirements()[j].threshold < this.requirements()[j].objective) || (+this.configurations()[i].variables[k].Value > this.requirements()[j].objective && +this.configurations()[i].variables[k].Value < this.requirements()[j].threshold && this.requirements()[j].threshold > this.requirements()[j].objective)){
								green = true;
								break;
							}
							if((+this.configurations()[i].variables[k].Value > this.requirements()[j].objective && this.requirements()[j].threshold < this.requirements()[j].objective) || (+this.configurations()[i].variables[k].Value < this.requirements()[j].objective && this.requirements()[j].threshold > this.requirements()[j].objective)){
								blue = true;
								break;
							}
							gray = true;
						}
					}
					if(!found){
						gray = true;
					}
				}
				if(red){
					this.configurations()[i].requirementsResults = '#BB1F1F'; //red
				}
				if(!red && gray){
					this.configurations()[i].requirementsResults = '#aaa'; //gray
				}
				if(!red && !gray && green){
					this.configurations()[i].requirementsResults = '#83BE6C'; //lightgreen
				}
				if(!red && !gray && !green && blue){
					this.configurations()[i].requirementsResults = '#65A0DF'; //lightblue
				}
			}
			return true;
		}
		else{
			return false;
		}
	};
	
	/**
     *  colorCodeDropDownBox
     *
     *  type: computed function
     */
	this.colorCodeDropDownBox = ko.computed(function () {
		if(this.listOfConfigurations().length > 0 && this.showRequirementTab()){
			this.selectedConfiguration();
			this.dataproviders.requirementsList();
			this.calculateRequirementsAnalysis();
			var elements = $('select.colorCode option');
			for(var i = 0; i < elements.length; i++){
				for(var j = 0; j < this.configurations().length; j++){
					if($(elements[i]).html() === this.configurations()[j].design){
						$(elements[i]).css('color',this.configurations()[j].requirementsResults);
						break;
					}
				}
			}
		}
	}, this);
	
	/**
     *  colorCodeDropDownBox2
     *
     *  type: computed function
     */
	this.colorCodeDropDownBox2 = function () {
		if(this.listOfConfigurations().length > 0 && this.showRequirementTab()){
			this.selectedConfiguration();
			this.dataproviders.requirementsList();
			this.calculateRequirementsAnalysis();
			var elements = $('select.colorCode option');
			for(var i = 0; i < elements.length; i++){
				for(var j = 0; j < this.configurations().length; j++){
					if($(elements[i]).html() === this.configurations()[j].design){
						$(elements[i]).css('color',this.configurations()[j].requirementsResults);
						break;
					}
				}
			}
		}
	};
	
	////////////////////////////// mw /////////////////////////
	
	this.showFV = ko.computed(function () {
		if(self.showExperimental()){
			return true;
		}
		return false;
	}, this); 

	/////////////////////// end of mw ///////////////////////////////
	/**
	 * showPCC
	 * checks to see if there is PCC data in the currently selected configuration/testbench
	 * if there is no data, an error box will appear
	 */
	this.showPCC = ko.computed(function () {
		if(self.pccResults().length > 0  || self.showExperimental()){
			return true
		}
		else{
			return false;
		}
	}, this); 

	this.showRankings = ko.computed(function () {
		if((self.vfMode() && !self.disableLeaderBoard())  || self.showExperimental()){
			return true
		}
		else{
			return false;
		}
	}, this); 
	
	/**
     *  scoresFromVF
     *
     *  type: observable
     *
     *  This array consists of other arrays that contain scoring
     *  information.
     */
	this.scoresFromVF = ko.observable({
			"history" : [],
			"rankings" : {},
			"teams" : {},
			"types" : []
		})
	
	/**
     *  scores
     *
     *  type: computed
     *
     *  This array consists of other arrays that contain scoring
     *  information.
     */
	this.scores = ko.computed(function () {
    console.log("vf-dataprovider: scores: enter");
    
		var out = {
			"categories" : clone(this.requirementCategories()),
			"history" : clone(this.scoresFromVF().history),
			"rankings" : {},
			"teams" : [],
			"teamsNames" : [],
			"teamsURLs" : [],
			"types" : clone(this.scoresFromVF().types)
		};
				
		//fill in teams and teamsURLs
		var teamsOrigin = clone(this.scoresFromVF().teams);
		var teams = Object.keys(teamsOrigin);
		for(var i = 0; i < teams.length; i++){
			out.teams.push(teams[i]);
			out.teamsNames.push(teamsOrigin[teams[i]].longName);
			out.teamsURLs.push(teamsOrigin[teams[i]].url);
		}
		
		// replace designId with design name in history
		for(var i = 0; i < out.history.length; i++){
			for(var j = 0; j < self.configurations().length; j++){
				if(self.configurations()[j].designId == 'CID'+out.history[i].design){
					out.history[i].design = self.configurations()[j].design;
					out.history[i].designId = out.history[i].designId;
				}
			}
		}
		// convert array of arrays into array of objects in rankings
		var rankings = clone(this.scoresFromVF().rankings)
		for(var j = 0; j < out.types.length; j++){
			out.rankings[out.types[j]] = {};
    		for(var i = 0; i < rankings[out.types[j]].length; i++){
    			out.rankings[out.types[j]][rankings[out.types[j]][i][0]] = rankings[out.types[j]][i][1];
    		}
		}

    // Find score range - 0..max_score
    var max_score = self.requirementsStructure().maximumScore;
    console.log("score range: 0.."+max_score);

    // Set the color for the score
    console.log("create design_list");
    console.log(out);
    var design_list = {};
    for (i=0; i<out.history.length; i++) {
        design = out.history[i].design;
        timestamp = out.history[i].timestamp;
        score = out.history[i]["Aggregated Score"];
        if (design_list.hasOwnProperty(design)) {
            if (design_list[design].timestamp < timestamp) {
                design_list[design].timestamp = timestamp;
                design_list[design].score = score;
            }
        } else {
            design_list[design] = {"score":score,
                                    "timestamp":timestamp,
                                    "color":"#555555"};
            console.log(design_list[design]);
        }
    }
    console.log(design_list);
      
    // Cull the design list
    console.log("cull design_list");
    var keys = [];
    var found;
    for (key in design_list) {
        keys.push(key);
    }
    for (i=0; i<keys.length; i++) {
        console.log("key="+keys[i]);
        found = false;
        for (j=0; j<self.configurations().length; j++) {
            if (self.configurations()[j].design === keys[i]) {
                found = true;
                
                // Now set color
                self.configurations()[j].score_color = self.icolor(design_list[key].score / max_score);
                break;
            }
        }
        // Delete score keys that are not in configurations
        if (!found) {
            delete design_list[keys[i]];
        }
    }
    console.log(design_list);
    
    console.log(out);

		return out;
	},this);
    
        
    /**
     *  scoresAvailble
     *
     *  type: computed
     *
     *  output: boolean - true=scores are available
     *
     *  This is a computed function that returns a boolean value
     *  that indicates whether the scores have been loaded.
     */
	this.scoresAvailble = ko.computed(function () {
		if(this.vfMode() || this.showExperimental()){
			if(self.scores().history.length > 0){
				return true;
			}
			else{
				return false;
			}
		}
		return false;
	},this);
	
		
    /**
     *  getScores
     *
     *  type: computed
     *
     *  output: null
     *
     */
	this.getScores = function () {
		var self = this;
		if(this.vfMode()){
			// get requirement id
			var reqId = this.currentRequirementsId();
			
			// A local reference to the ajaxHandlers variable
			var h = this.ajaxHandlers;
			var n = this.ajaxURLs;
		
			// Retrieve the scoring data, save ajax to handler array (fix path first)
			//var scoresPath = projectLocation.split("/").slice(0,-1).join("/") + "/scores.json?format=raw"; // fake test file - remove  /// 
            var scoresPath = "/rest/"+self.vf_neighborhood()+"/standings/get_current_standings?team="+projectLocation.split('/')[2]+"&req_id="+reqId;
            console.log("vf-dataproviders: loadDataFiles: ajax: scoresPath="+scoresPath);
            n.push(scoresPath);
            h.push($.ajax({
                url : scoresPath,
                dataType : 'json',
                self : self,
                success : function (scores, status, xhr) {
                    console.log("vf-provider: loadDataFiles: success: "+scoresPath);
                    self.scoresFromVF(scores[self.currentRequirementsId()]);
                },
                complete : function(){
                    self.loadPending();
                },
				error: function(jqXHR, textStatus, errorThrown){
					if(jqXHR.statusText == "OK"){jqXHR.statusText = textStatus;}
					self.loadPending();
				}
            }));
        }
		else{
			// get requirement id
			var reqId = this.currentRequirementsId();
			
			// A local reference to the ajaxHandlers variable
			var h = this.ajaxHandlers;
			var n = this.ajaxURLs;
		
			// Retrieve the scoring data, save ajax to handler array (fix path first)
			var scoresPath = projectLocation.split("/").slice(0,-1).join("/") + "/scores.json?format=raw"; // fake test file - remove  /// 
            //var scoresPath = "/rest/"+self.vf_neighborhood()+"/standings/get_current_standings?team="+projectLocation.split('/')[2]+"&req_id="+reqId;
            console.log("vf-dataproviders: loadDataFiles: ajax: scoresPath="+scoresPath);
            n.push(scoresPath);
            h.push($.ajax({
                url : scoresPath,
                dataType : 'json',
                self : self,
                success : function (scores, status, xhr) {
                    console.log("vf-provider: loadDataFiles: success: "+scoresPath);
                    self.scoresFromVF(scores[Object.keys(scores)[0]]);
                },
                complete : function(){
                    self.loadPending();
                },
				error: function(jqXHR, textStatus, errorThrown){
					if(jqXHR.statusText == "OK"){jqXHR.statusText = textStatus;}
					self.loadPending();
				}
            }));
		}
	};
   
    /**
     *  thresholds
     *
     *  type: computed array
     *
     *  The contents of this array are a filtered list of the
     *  self.requirements() computed array.  The contents of this
     *  computed array are indexed by the requirements metricName
     *  and the value is the threshold value from the requirements
     *  entry.
     */
		this.thresholds = ko.computed(function () {
			var out = {};
			for (var i = 0; i < this.requirements().length; i++) {
				out[this.requirements()[i].testBench + "_" + this.requirements()[i].metricName] = this.requirements()[i].threshold;
			}
			return out;
		}, this);
   
    /**
     *  objectives
     *
     *  type: computed array
     *
     *  The contents of this array are a filtered list of the
     *  self.requirements() computed array.  The contents of this
     *  computed array are indexed by the requirements metricName
     *  and the value is the objective value from the requirements
     *  entry.
     */
		this.objectives = ko.computed(function () {
			var out = {};
			for (var i = 0; i < this.requirements().length; i++) {
				out[this.requirements()[i].testBench + "_" + this.requirements()[i].metricName] = this.requirements()[i].objective;
			}
			return out;
		}, this);
		
	
	//// Auto start GUI ////
	// (function () {
		// for (var i = 0; i < self.configurations().length; i++) {
			// self.configurations()[i].isActive.subscribe(function (newValue) {
				// $(document).trigger('dp-ActiveToggled');
			// });
		// }
		// for (var j = 0; j < self.configurations().length; j++) {
			// self.variables()[j].isActive.subscribe(function (newValue) {
				// $(document).trigger('dp-ActiveToggled');
			// });
		// }
		// self.byVariable.subscribe(function (newValue) {
			// $(document).trigger('dp-ActiveToggled');
		// });
		// self.configurations.subscribe(function (newValue) {
			// $(document).trigger('dp-ActiveToggled');
		// });
	// })();
	
	// current Pareto Frontier Axis
   
    /**
     *  pfXaxis
     *
     *  type: observable
     *
     *  The contents of this variable is the x-axis variable.
     */
	this.pfXaxis = ko.observable("Vehicle Unit Cost");
   
    /**
     *  pfYaxis
     *
     *  type: observable
     *
     *  The contents of this variable is the y-axis variable.
     */
	this.pfYaxis = ko.observable("Automotive Performance");
   
    /**
     *  isDemo
     *
     *  type: observable
     *
     *  A flag.
     */
	this.isDemo = ko.observable(true);
   
    /**
     *  demos
     *
     *  type: object
     *
     *  An object containing the observables multiVariate and parallelAxis.
     */
	this.demos = {
		profiler : ko.observable(),
		parallelAxis : ko.observable()
	};
	

    /*
        Load the data files
     */
     
    /**
     *  numberOfFilesToLoad
     *
     *  type: variable
     *
     *  This variable contains the number of files to be loaded.
     */

	this.numberOfFilesToLoad = 2;
   
    /**
     *  ajaxHandlers
     *
     *  type: array
     *
     *  This array will contain the ajax references.
     */
    this.ajaxHandlers = ko.observableArray([]);
    this.ajaxURLs = ko.observableArray([]);
    this.loadSummary = ko.observableArray([]);
    this.loadError = ko.observable(false);
    this.limitSummary = ko.observableArray([]);
    this.limitError = ko.observable(false);
    this.numberOfLimitExceededDesigns = ko.observable(0);
    this.numberOfLimitExceededTestBenches = ko.observable(0);
    
	/**
     *  isInViolation
     *
     *  type: function
     *
     *  output: check if config is in violation of a limit
     *
     */
    this.isInViolation = function (config) {
			for(i = 0; i < this.limitSummary().length; i++){
				if(config === this.limitSummary()[i].design){
					return true;
				}
			}
			return false;
		},

	/**
     *  formal verification
     *
     */
    this.formalverification = ko.observableArray([]);
	this.qrSelectedX = ko.observable('');
	this.qrSelectedY = ko.observable('');
	
    /**
     *  numberOfFiles
     *
     *  type: computed
     *
     *  output: Number of Files
     *
     */
    this.numberOfFiles = ko.computed(function () {
			return this.ajaxHandlers().length;
		}, this);
		
	
    /**
	 *  pccDataFileProcessingFunctionsResultsSummaries
	 *
	 *  type: functions
	 *
	 *  input: none
	 *
	 *  output: none
	 */
	this.pccDataFileProcessingFunctionsResultsSummaries = function(report){
		// build pccObj
		var keys = Object.listKeys(report.PCCResults);
		for(var k = 0; k < keys.length; k++){
			var data = {};
			data[keys[k]] = report.PCCResults[keys[k]];
			var pccObj = {
				design : report.DesignName,
				designId: 'CID' + report.DesignID.split("-").join("").split("{").join("").split("}").join(""),
				time : typeof report.Time === 'undefined' ? report.Created : report.Time,
				testBench : report.TestBench,
				distExist : false,
				sensExist : false,
				PCCResults: {
					byMethod: data,
					byAttr: Object.invertObjOfObjs(data)
				},
				variables : []
			};
			
			if (typeof pccObj.PCCResults.byAttr != "undefined")
			{
				if ('JointPCC' in pccObj.PCCResults.byAttr) //all UP methods return JointPCC array
				{
					pccObj.distExist = true;
				}
				
				if ('FirstOrderSensitivity' in pccObj.PCCResults.byAttr) //all SA methods return FirstOrderSensitivity obj
				{
					pccObj.sensExist = true;
				}
			}
			
			for (var j = 0; j < report.Metrics.length; j++)
			{
				pccObj.variables.push(report.Metrics[j].Name);
			}
			
			for (var j = 0; j < report.Parameters.length; j++)
			{
				pccObj.variables.push(report.Parameters[j].Name + " (P)");
			}

			// check if design / test bench pair exist in allPccResults()
			var added = false;
			if (self.allPccResults().length > 0) {
				for(var i = 0; i < self.allPccResults().length; i++){
					if(self.allPccResults()[i][0].designId === pccObj.designId && self.allPccResults()[i][0].testBench === pccObj.testBench){
						self.allPccResults()[i].push(pccObj);
						added = true;
						if(pccObj.distExist){
							self.activeUpPccResultsIds()[i] = self.allPccResults()[i].length - 1;
						}
						if(pccObj.sensExist){
							self.activeSaPccResultsIds()[i] = self.allPccResults()[i].length - 1;
						}
						break;
					}
				}
			}
			if(!added){
				self.allPccResults.push([pccObj]);
				if(pccObj.distExist){
					self.activeUpPccResultsIds.push(0);
					self.activeSaPccResultsIds.push(null);
				}
				if(pccObj.sensExist){
					self.activeUpPccResultsIds.push(null);
					self.activeSaPccResultsIds.push(0);
				}
			}

			//force update
			self.activeUpPccResultsIds(self.activeUpPccResultsIds());
			self.activeSaPccResultsIds(self.activeSaPccResultsIds());
		}
		
		self.activeUpPccResultsIds(self.activeUpPccResultsIds());
		self.activeSaPccResultsIds(self.activeSaPccResultsIds());
	};

	/**
	 *  dataFileProcessingFunctionsResultsSummaries
	 *
	 *  type: functions
	 *
	 *  input: none
	 *
	 *  output: none
	 */
	this.processResultsSummaries = function(report, textStatus, jqXHR, url){
		// allPccResults
		self.pccDataFileProcessingFunctionsResultsSummaries(report);
				
		var config = {
			design : report.DesignName,
			designId: 'CID' + report.DesignID.split("-").join("").split("{").join("").split("}").join(""),
			time : typeof report.Time === 'undefined' ? report.Created : report.Time,
			isSelected : false,
			isActive : true,
			testBench : report.TestBench,
			variables : [],
			responses : report.responses,
			PCCResults: report.PCCResults,
			requirementsResults : 'white',
			visualizationArtifacts : [],
			visualizationArtifactTestBench : []
		}
		
		//miyako temp
		/*config.visualizationArtifacts.push("/darpa-test/data/VisualizationArtifacts/results/zlocv4zr/plots/Superboltsandtearout.mov");
		config.visualizationArtifacts.push("/darpa-test/data/VisualizationArtifacts/results/zlocv4zr/plots/wigleyhullwithWaves.m4v");
		config.visualizationArtifacts.push("/darpa-test/data/VisualizationArtifacts/results/zlocv4zr/plots/FloatingBox.m4v");
		config.visualizationArtifacts.push("/darpa-mw/demo/img/Picture5-gif.gif");
		config.visualizationArtifacts.push("/darpa-mw/demo/img/Picture5-jpg.jpg");
		config.visualizationArtifacts.push("/darpa-mw/demo/img/Picture5-png.png");
		*/
		//config.visualizationArtifacts.push("/darpa-mw/demo/stl/stlfiles/solid.stl");
		/*config.visualizationArtifacts.push("/darpa-mw/demo/img/sintel.mp4");
		config.visualizationArtifacts.push("http://www.asdl.gatech.edu");         
		config.visualizationArtifacts.push("http://www.cnn.com/video/api/embed.html#/video/bestoftv/2014/02/28/morning-minute-2-28-newday-ap.cnn-ap");                               
		config.visualizationArtifacts.push("http://www.youtube.com/embed/_Pr_fpbAok8?feature=player_embedded");
		config.visualizationArtifacts.push("/darpa-mw/demo/img/sintel.mp4");
		config.visualizationArtifacts.push("http://www.youtube.com/embed/A-5I3cYhZ2U?feature=player_embedded");
		
		*/
		
		//config.visualizationArtifacts.push("/darpa-mw/demo/stl/stlfiles/Peacock_USB-V2.stl");
		//config.visualizationArtifacts.push( "/darpa-mw/demo/stl/stlfiles/XmasSet_Coal_X1.stl");
		//config.visualizationArtifacts.push("/darpa-mw/demo/stl/stlfiles/nx.stl");
		
		if(report.VisualizationArtifacts){
			for (var i = report.VisualizationArtifacts.length - 1; i >= 0; i--) {
				config.visualizationArtifacts.push(url.split('/').slice(0,-1).join('/') + '/' + report.VisualizationArtifacts[i].Location);
				config.visualizationArtifactTestBench.push(report.TestBench);
			};
		}
		
		// Add limitInfo
		if(isSet('report.LimitChecks', {report:report})){	
		  config.limitInfo = report.LimitChecks;
		}
			
		// Add the Parameters to the variable list for the configuration
		var leastSigFigs = 1000;
		for (var i = 0; i < report.Parameters.length; i++) {
			if(report.Parameters[i].Value !== ''){
				// convert all true or false to 1 and 0
				if(report.Parameters[i].Value === true || report.Parameters[i].Value === "true" || report.Parameters[i].Value === "True"){
					report.Parameters[i].Value = 1;
				}
				if(report.Parameters[i].Value === false || report.Parameters[i].Value === "false" || report.Parameters[i].Value === "False"){
					report.Parameters[i].Value = 0;
				}
				report.Parameters[i].type = 'parameter';
				report.Parameters[i].testBench = report.TestBench;
				report.Parameters[i].PCCResults = report.PCCResults;
				config.variables.push(report.Parameters[i]);
				
				// Calculate Significant Figures
				var sigFig = new SignificantFigures(report.Parameters[i].Value.toString());
				if(+sigFig.sigFigs < +leastSigFigs){
					leastSigFigs = sigFig.sigFigs;
				}
			} else{
				jqXHR.statusText = 'Contains blank values';
			}
		}
			
		// Append a (P) to the variable names of the parameters
		// Parameters are the only contents of the variable list at this point
		for (var i = 0; i < config.variables.length; i++) {
			config.variables[i].Name = config.variables[i].Name + ' (P)';
		}
			
		// Add the Metrics to the variable list for the configuration
		for (var i = 0; i < report.Metrics.length; i++) {
			if(report.Metrics[i].Value !== ''){
				// convert all true or false to 1 and 0
				if(report.Metrics[i].Value === true || report.Metrics[i].Value === "true" || report.Metrics[i].Value === "True"){
					report.Metrics[i].Value = 1;
				}
				if(report.Metrics[i].Value === false || report.Metrics[i].Value === "false" || report.Metrics[i].Value === "False"){
					report.Metrics[i].Value = 0;
				}
				report.Metrics[i].type = 'metric';
				report.Metrics[i].testBench = report.TestBench;
				report.Metrics[i].PCCResults = report.PCCResults;

				if (report.TierLevel) {
					report.Metrics[i].TierLevel = report.TierLevel;
				};
				
				// set Significant Figures
				report.Metrics[i].Value = +displaySigFigs(+report.Metrics[i].Value,leastSigFigs+2,-999,false);
				config.variables.push(report.Metrics[i]);
			} else{
				jqXHR.statusText = 'Contains blank values';
			}

			if(report.Metrics[i].VisualizationArtifacts){
				for (var j = report.Metrics[i].VisualizationArtifacts.length - 1; j >= 0; j--) {
					config.visualizationArtifacts.push(url.split('/').slice(0,-1).join('/') + '/' + report.Metrics[i].VisualizationArtifacts[j].Location);
					config.visualizationArtifactTestBench.push(report.TestBench);
				};
			}
		}
			
		// Add the LimitChecks, where the limit has been exceeded, to the variable list for the configuration
		if(isSet('report.LimitChecks', {report:report})){	
		  for (var i = 0; i < report.LimitChecks.length; i++) {
			if(report.LimitChecks[i].LimitExceeded){
			  console.log("LimitChecks: "+report.LimitChecks[i].LimitExceeded);
			  var limitInfo = {};
			  limitInfo.type = 'limit';
			  limitInfo.ActualValue = report.LimitChecks[i].ActualValue;
			  limitInfo.Value = report.LimitChecks[i].Value;
			  limitInfo.LimitName = report.LimitChecks[i].LimitName;
			  limitInfo.testBench = report.TestBench;
			  limitInfo.instance = report.LimitChecks[i].ComponentInstanceName;
			  limitInfo.LimitFullPath = report.LimitChecks[i].LimitFullPath;
			  limitInfo.VariableName = report.LimitChecks[i].VariableName;
			  limitInfo.VariableFullPath = report.LimitChecks[i].VariableFullPath;
			  console.log(limitInfo);
			  config.variables.push(limitInfo);
			}
		  }
		}
		
		// Add variable file locations
		for (var i = 0; i < config.variables.length; i++) {
			config.variables[i].location = url; //.split('/').slice(0,-1).join('/');
		}

		//load Formal verifcation Data
		if(isSet('report.FormalVerification.length', {report:report})){
			for(var i = 0; i < report.FormalVerification.length; i++){
				report.FormalVerification[i].DesignName = report.DesignName;
				report.FormalVerification[i].DesignID = report.DesignID;
				report.FormalVerification[i].TestBench = report.TestBench;
				report.FormalVerification[i].DesignSpace = '';
				self.formalverification.push(report.FormalVerification[i]);
				this.qrSelectedX(report.FormalVerification[i].ReqName);
  				this.qrSelectedY(report.FormalVerification[i].DesignName);
			}
		}
		
		// Add the configuration entry to the list
		self.allConfigurations.push(config);
	};
	
	/**
	*	This helper function will concatenate two URLs and finish by cleaning out unnecessary characters within the URL
	*/
	this.urlHelper = function(url_1, url_2){
		var visualizerMode = getParameterByName('local', document.originalURL);
		var Url = url_1.split('/').slice(0, -1).join('/') + '/' + url_2;
		Url = Url.split('\\').join('/');
		Url = encodeURI(Url);
		if(visualizerMode === "true"){
			Url = window.location.pathname.split('/').slice(0,-1).join('/') + Url;
		}
		Url = Url.split('/./').join('/');
		return Url;
	}
   
/**
 *  loadDataFiles
 *
 *  type: function
 *
 *  input: none
 *
 *  output: none
 *
 *  This function will retrieve the various data files from the server via ajax
 *  calls.  The data is installed into the appropriate variables (or observable).
 */
this.loadDataFiles = function(){
		var self = this;
        
    // A local reference to the ajaxHandlers variable
    var h = this.ajaxHandlers;
    var n = this.ajaxURLs;

    console.log("vf-dataproviders: loadDataFiles: enter");
		
    self.designHash = CryptoJS.SHA256(projectLocation);

    console.log("projectLocation="+projectLocation);
    console.log("designHash="+self.designHash);
    visualizerMode = getParameterByName('local', document.originalURL);
        
    // Retrieve the state
    this.retrieveState();
		
    // Retrieve the manifest.json file and set the version in the HTML
    console.log("vf-dataproviders: loadDataFiles: ajax: manifest.json");
    n.push('./manifest.json');
		h.push($.ajax({
        url : './manifest.json',
        dataType : 'json',
        context : this,
        success : function (data) {
            console.log("vf-dataproviders: loadDataFiles: success: manifest.json");
            $('#version').html('Version ' + data.version);
			$('title').html($('title').html() + ' ' + data.version);
			if(getParameterByName('theme', document.originalURL) !== ''){
				self.theme(getParameterByName('theme', document.originalURL));
			}
			else{
				self.theme(data.theme);
			}
			if(getParameterByName('title', document.originalURL) !== ''){
				self.subTitle(getParameterByName('title', document.originalURL));
			}
            self.loadPending();
        },
        error: function(jqXHR, textStatus, errorThrown){
            console.error("vf-dataproviders: loadDataFiles: error(manifest.json): ",errorThrown);
            if(jqXHR.statusText == "OK"){jqXHR.statusText = textStatus;}
            self.loadPending();
        }
    }));
	
    // Retrieve the meta-data files
    console.log("vf-dataproviders: loadDataFiles: ajax(projectLocation): "+projectLocation);
    n.push(projectLocation);
    h.push($.ajax({
        url : projectLocation,
        dataType : 'text',
        self : self,
        success : function (avmproject, status, xhr) {
        	// hack to catch NaN from GME tools
        	avmproject = avmproject.replace(/NaN/g,"null");
        	avmproject = JSON.parse(avmproject);
        	// remove wrapping if wrapped
        	if(isSet('avmproject.Project',{'avmproject':avmproject})){
        		avmproject = avmproject.Project;
        	}

            console.log("vf-dataproviders: loadDataFiles: success(projectLocation): "+projectLocation);
            console.log("\nvf-dataproviders: loadDataFiles: success(projectLocation): Results(level 1) - imax="+avmproject.Results.UrlHints.length);
            for (var i = 0; i < avmproject.Results.UrlHints.length; i++) {
				if(typeof avmproject.Results.UrlHints[i] === 'string'){
									
					// Retrieve
					var iUrl = self.urlHelper(projectLocation, avmproject.Results.UrlHints[i]);
					console.log("vf-dataproviders: loadDataFiles: ajax(iUrl): "+iUrl+"?format=raw");
					n.push(iUrl);
					h.push($.ajax({
						url : iUrl + '?format=raw',
						dataType : 'json',
						self : self,
						success : function (metaResult, status, xhr) {
							console.log("vf-dataproviders: loadDataFiles: success(iUrl): "+iUrl+"?format=raw");
							console.log("vf-dataproviders: loadDataFiles: success(iUrl): Results(level 2) - jmax="+	metaResult.Results.length);
										
							for (var j = 0; j < metaResult.Results.length; j++) {
								var jUrl = self.urlHelper(iUrl, metaResult.Results[j].Summary);
								console.log("vf-dataproviders: loadDataFiles: ajax(jUrl): "+jUrl+"?format=raw");
								n.push(jUrl);
								h.push($.ajax({
									url : jUrl + '?format=raw',
									dataType : 'json',
									self : self,
									success : function(report, textStatus, jqXHR){
										self.processResultsSummaries(report, textStatus, jqXHR, this.url)
									},
									complete: function(){
										console.log("vf-dataproviders: loadDataFiles: complete(jUrl): "+this.url+"?format=raw");
										self.loadPending();
									},
									error: function(jqXHR, textStatus, errorThrown){
										if(jqXHR.statusText == "OK"){jqXHR.statusText = textStatus;}
										self.loadPending();
									}
								}));
							}
						},
						complete: function(){
							console.log("vf-dataproviders: loadDataFiles: complete(iUrl): "+iUrl+"?format=raw");
							self.loadPending();
						},
						error: function(jqXHR, textStatus, errorThrown){
							if(jqXHR.statusText == "OK"){jqXHR.statusText = textStatus;}
								self.loadPending();
						}
					}));
				}
				else{
					for (var j = 0; j < avmproject.Results.UrlHints[i].Results.length; j++) {
						if(typeof avmproject.Results.UrlHints[i].Results[j].Summary !== "string"){
							self.processResultsSummaries(avmproject.Results.UrlHints[i].Results[j].Summary, status, xhr, projectLocation);
						}
					}
				}
				//vf-dataproviders: loadDataFiles: success(projectLocation): Requirements - kmax="+avmproject.Requirements.UrlHints.length);
				if (self.vfMode()) {
					// Read requirements from the server (this depends on the server placing the format parameter in the parameters)
					console.log("REQUIREMENTS from server");
					kUrl = "/rest/"+self.vf_neighborhood()+"/standings/get_requirements?return_all=true&format=raw";
					console.log("vf-dataproviders: loadDataFiles: ajax(kUrl): "+kUrl+"?format=raw");
					n.push(kUrl);
					h.push($.ajax({
						url : kUrl,
						dataType : 'json',
						self : self,
						success : function (req, status, xhr) {
							console.log("vf-dataproviders: loadDataFiles: success(kUrl): "+kUrl+"?format=raw");
							self.allRequirementsFromVF(req);
						},
						complete: function(){
							console.log("vf-dataproviders: loadDataFiles: complete(kUrl): "+kUrl+"?format=raw");
							self.loadPending();
						},
						error: function(jqXHR, textStatus, errorThrown){
								
							if(jqXHR.statusText == "OK"){
								jqXHR.statusText = textStatus;
							}
							self.loadPending();
						}
					}));
				} else {
					// Read requirements from the local files
					console.log("REQUIREMENTS from file");
					for (var k = 0; k < avmproject.Requirements.UrlHints.length; k++) {
						if(typeof avmproject.Requirements.UrlHints[k] === 'string'){
							var kUrl = self.urlHelper(projectLocation, avmproject.Requirements.UrlHints[k]);
							console.log("vf-dataproviders: loadDataFiles: ajax(kUrl): "+kUrl+"?format=raw");
							n.push(kUrl);
							h.push($.ajax({
								url : kUrl,
								dataType : 'json',
								self : self,
								success : function (req, status, xhr) {
									console.log("vf-dataproviders: loadDataFiles: success(kUrl): "+kUrl+"?format=raw");
									self.allRequirementsFromVF({requirements:req});
								},
								complete: function(){
									console.log("vf-dataproviders: loadDataFiles: complete(kUrl): "+kUrl+"?format=raw");
									self.loadPending();
								},
								error: function(jqXHR, textStatus, errorThrown){
										
									if(jqXHR.statusText == "OK"){
										jqXHR.statusText = textStatus;
									}
									self.loadPending();
								}
							}));
						}
						else{
							self.allRequirementsFromVF({requirements:avmproject.Requirements.UrlHints[k]});
						}
					}
				}
				}
            console.log("\nvf-dataproviders: loadDataFiles: success(projectLocation): Configurations - lmax="+
                avmproject.Configurations.length);
            for (var l = 0; l < avmproject.Configurations.length; l++) {
                if(typeof avmproject.Configurations[l] === 'string'){
                	if(avmproject.Configurations[l].indexOf('&lt;') === -1 && self.vfMode() === false){
		                var lUrl = self.urlHelper(projectLocation, avmproject.Configurations[l]);
		                console.log("vf-dataproviders: loadDataFiles: ajax(lUrl): "+lUrl+"?format=raw");
		                n.push(lUrl);
		                h.push($.ajax({
		                    url : lUrl + '?format=raw',
		                    dataType : 'text',
		                    self : self,
		                    success : function (com, status, xhr) {
		                        console.log("vf-dataproviders: loadDataFiles: success(lUrl): "+lUrl+"?format=raw");
		                        try{
		                        	com = JSON.parse(com);
		                        }
		                        catch(err){
		                        	com = $.xml2json(com);
		                        }
		                        

		                        var out = {
		                            design: com.Name,
		                            cid: com.Name,
		                            componets: []
		                        };
									
		                        var searchInObject = function (x) {
		                            
		                        	// for JSON files
		                            if(typeof x.ComponentInstances != 'undefined'){
		                                for(var i = 0; i < x.ComponentInstances.length; i++){
		                                    out.componets.push({
												id: x.ComponentInstances[i].SrcDesignSpaceContainerID,
		                                        type: '',
		                                        name: '',
												containerType: ''
		                                    });
		                                }
		                            }

		                            // for XML files
		                            if(typeof x.ComponentInstance != 'undefined'){
		                            	if (Array.isArray(x.ComponentInstance)) {
			                                for(var i = 0; i < x.ComponentInstance.length; i++){
			                                    out.componets.push({
													id: x.ComponentInstance[i].DesignSpaceSrcComponentID,
			                                        type: '',
			                                        //typeId: x.ComponentInstance[i].ComponentID,
			                                        name: '',
													containerType: ''
			                                    });
			                                }
			                            }
			                            else{
			                            	out.componets.push({
												id: x.ComponentInstance.DesignSpaceSrcComponentID,
		                                        type: '',
		                                        //typeId: x.ComponentInstance.ComponentID,
		                                        name: '',
												containerType: ''
		                                    });
			                            }
		                                
		                            }
										
		                            if (Array.isArray(x)) {
		                                for (var i = 0; i < x.length; i++) {
		                                    searchInObject(x[i]);
		                                }
		                            }
		                            if (typeof x['Containers'] != 'undefined') {
		                                searchInObject(x['Containers']);
		                            }
		                            if (typeof x['Container'] != 'undefined') {
		                                searchInObject(x['Container']);
		                            }
		                            if (typeof x['RootContainer'] != 'undefined') {
		                                searchInObject(x['RootContainer']);
		                            }
										
		                        }
		                        searchInObject(com);
		                        self.componetsRaw.push(out);
		                    },
		                    complete: function(){
		                        console.log("vf-dataproviders: loadDataFiles: complete(lUrl): "+lUrl+"?format=raw");
		                        self.loadPending();
		                    },
		                    error: function(jqXHR, textStatus, errorThrown){
		                        if(jqXHR.statusText == "OK"){jqXHR.statusText = textStatus;}
		                        self.loadPending();
		                    }
		                }));
					}
					else{
						try{
		                        	com = JSON.parse(avmproject.Configurations[l]);
		                        }
		                        catch(err){
		                        	com = $.xml2json(avmproject.Configurations[l].replace(/&gt;/g,'>').replace(/&lt;/g,'<'));
		                        }
		                        

		                        var out = {
		                            design: com.Name,
		                            cid: com.Name,
		                            componets: []
		                        };
									
		                        var searchInObject = function (x) {
		                            
		                        	// for JSON files
		                            if(typeof x.ComponentInstances != 'undefined'){
		                                for(var i = 0; i < x.ComponentInstances.length; i++){
		                                    out.componets.push({
												id: x.ComponentInstances[i].SrcDesignSpaceContainerID,
		                                        type: '',
		                                        name: '',
												containerType: ''
		                                    });
		                                }
		                            }

		                            // for XML files
		                            if(typeof x.ComponentInstance != 'undefined'){
		                                if (Array.isArray(x.ComponentInstance)) {
			                                for(var i = 0; i < x.ComponentInstance.length; i++){
			                                    out.componets.push({
													id: x.ComponentInstance[i].DesignSpaceSrcComponentID,
			                                        type: '',
			                                        //typeId: x.ComponentInstance[i].ComponentID,
			                                        name: '',
													containerType: ''
			                                    });
			                                }
			                            }
			                            else{
			                            	out.componets.push({
												id: x.ComponentInstance.DesignSpaceSrcComponentID,
		                                        type: '',
		                                        //typeId: x.ComponentInstance.ComponentID,
		                                        name: '',
												containerType: ''
		                                    });
			                            }
		                            }
										
		                            if (Array.isArray(x)) {
		                                for (var i = 0; i < x.length; i++) {
		                                    searchInObject(x[i]);
		                                }
		                            }
		                            if (typeof x['Containers'] != 'undefined') {
		                                searchInObject(x['Containers']);
		                            }
		                            if (typeof x['Container'] != 'undefined') {
		                                searchInObject(x['Container']);
		                            }
		                            if (typeof x['RootContainer'] != 'undefined') {
		                                searchInObject(x['RootContainer']);
		                            }
										
		                        }
		                        searchInObject(com);
		                        self.componetsRaw.push(out);
					}
				}
				else{
					var com = avmproject.Configurations[l]
					var out = {
                            design: com.Name,
                            cid: com.Name,
                            componets: []
                        };
							
                        var searchInObject = function (x) {
                            // for JSON files
                            if(typeof x.ComponentInstances != 'undefined'){
                                for(var i = 0; i < x.ComponentInstances.length; i++){
                                    out.componets.push({
										id: x.ComponentInstances[i].SrcDesignSpaceContainerID,
                                        type: '',
                                        name: '',
										containerType: ''
                                    });
                                }
                            }
								
                            if (Array.isArray(x)) {
                                for (var i = 0; i < x.length; i++) {
                                    searchInObject(x[i]);
                                }
                            }
                            if (typeof x['Containers'] != 'undefined') {
                                searchInObject(x['Containers']);
                            }
								
                        }
                        searchInObject(com);
                        self.componetsRaw.push(out);
				}
            }
            
            console.log("\nvf-dataproviders: loadDataFiles: success(projectLocation): DesignSpaceModels - mmax="+
                avmproject.DesignSpaceModels.length);
            for (var m = 0; m < avmproject.DesignSpaceModels.length; m++) {
            	if(typeof avmproject.DesignSpaceModels[m] === 'string'){
            		if(avmproject.DesignSpaceModels[m].indexOf('&lt;') === -1 && self.vfMode() === false){
		                var mUrl = self.urlHelper(projectLocation, avmproject.DesignSpaceModels[m]);
		                console.log("vf-dataproviders: loadDataFiles: ajax(mUrl): "+mUrl+"?format=raw");
		                n.push(mUrl);
		                h.push($.ajax({
		                    url : mUrl + '?format=raw',
		                    dataType : 'text',
		                    self : self,
		                    success : function (com, status, xhr) {
		                        console.log("vf-dataproviders: loadDataFiles: success(mUrl): "+mUrl+"?format=raw");
		                        try{
		                        	com = JSON.parse(com);
		                        }
		                        catch(err){
		                        	com = $.xml2json(com);
		                        }

		                        var out = {
		                            design: com.Name,
		                            cid: com.Name,
		                            componets: []
		                        };
									
		                        var searchInObject = function (x) {
		                            // for JSON files
		                            if(typeof x.ComponentInstances != 'undefined'){
		                                for(var i = 0; i < x.ComponentInstances.length; i++){
		                                    out.componets.push({
												id: x.ComponentInstances[i].id.split('{').join('').split('}').join(''),
		                                        type: x.Name,
												typeId: x.id,
		                                        name: x.ComponentInstances[i].Name,
												containerType: x.$type.split('.').pop()
		                                    });
		                                }
		                            }

		                            // for XML files
		                            if(typeof x.ComponentInstance != 'undefined'){
		                            	if(typeof x.containerId === 'undefined'){
		                                	x.containerId = CryptoJS.SHA256(Math.random().toString()).toString();
		                                }
		                                if (Array.isArray(x.ComponentInstance)) {
			                                for(var i = 0; i < x.ComponentInstance.length; i++){
			                                    out.componets.push({
													id: x.ComponentInstance[i].ID,
													//componetId: x.ComponentInstance[i].ComponentID,
			                                        type: x.Name,
													typeId: x.containerId,
			                                        name: x.ComponentInstance[i].Name,
													containerType: x['xsi:type'].split(':').pop()
			                                    });
			                                }
			                            }
			                            else{
			                            	out.componets.push({
												id: x.ComponentInstance.ID,
												//componetId: x.ComponentInstance.ComponentID,
		                                        type: x.Name,
												typeId: x.containerId,
		                                        name: x.ComponentInstance.Name,
												containerType: x['xsi:type'].split(':').pop()
		                                    });
			                            }
		                            }
										
		                            if (Array.isArray(x)) {
		                                for (var i = 0; i < x.length; i++) {
		                                    searchInObject(x[i]);
		                                }
		                            }
		                            if (typeof x['Containers'] != 'undefined') {
		                                searchInObject(x['Containers']);
		                            }
		                            if (typeof x['Container'] != 'undefined') {
		                                searchInObject(x['Container']);
		                            }
		                            if (typeof x['RootContainer'] != 'undefined') {
		                                if(typeof x['RootContainer'].containerId === 'undefined'){
		                                	x['RootContainer'].containerId = CryptoJS.SHA256(Math.random().toString()).toString();
		                                }
		                                searchInObject(x['RootContainer']);
		                            }
										
		                        }
		                        searchInObject(com);
								self.designSpaceModels.push(out);
		                    },
		                    complete: function(){
		                        console.log("vf-dataproviders: loadDataFiles: complete(mUrl): "+mUrl+"?format=raw");
		                        self.loadPending();
		                    },
		                    error: function(jqXHR, textStatus, errorThrown){
									
		                        if(jqXHR.statusText == "OK"){jqXHR.statusText = textStatus;}
		                        self.loadPending();
		                    }
		                }));
					}
					else{
						try{
                        	com = JSON.parse(avmproject.DesignSpaceModels[m]);
                        }
                        catch(err){
                        	com = $.xml2json(avmproject.DesignSpaceModels[m].replace(/&gt;/g,'>').replace(/&lt;/g,'<'));
                        }

                        var out = {
                            design: com.Name,
                            cid: com.Name,
                            componets: []
                        };
							
                        var searchInObject = function (x) {
                            // for JSON files
                            if(typeof x.ComponentInstances != 'undefined'){
                                for(var i = 0; i < x.ComponentInstances.length; i++){
                                    out.componets.push({
										id: x.ComponentInstances[i].id.split('{').join('').split('}').join(''),
                                        type: x.Name,
										typeId: x.id,
                                        name: x.ComponentInstances[i].Name,
										containerType: x.$type.split('.').pop()
                                    });
                                }
                            }

                            // for XML files
                            if(typeof x.ComponentInstance != 'undefined'){
                            	if(typeof x.containerId === 'undefined'){
                                	x.containerId = CryptoJS.SHA256(Math.random().toString()).toString();
                                }
                                 if (Array.isArray(x.ComponentInstance)) {
			                                for(var i = 0; i < x.ComponentInstance.length; i++){
			                                    out.componets.push({
													id: x.ComponentInstance[i].ID,
													//componetId: x.ComponentInstance[i].ComponentID,
			                                        type: x.Name,
													typeId: x.containerId,
			                                        name: x.ComponentInstance[i].Name,
													containerType: x['xsi:type'].split(':').pop()
			                                    });
			                                }
			                            }
			                            else{
			                            	out.componets.push({
												id: x.ComponentInstance.ID,
												//componetId: x.ComponentInstance.ComponentID,
		                                        type: x.Name,
												typeId: x.containerId,
		                                        name: x.ComponentInstance.Name,
												containerType: x['xsi:type'].split(':').pop()
		                                    });
			                            }
                            }
								
                            if (Array.isArray(x)) {
                                for (var i = 0; i < x.length; i++) {
                                    searchInObject(x[i]);
                                }
                            }
                            if (typeof x['Containers'] != 'undefined') {
                                searchInObject(x['Containers']);
                            }
                            if (typeof x['Container'] != 'undefined') {
                                searchInObject(x['Container']);
                            }
                            if (typeof x['RootContainer'] != 'undefined') {
                                if(typeof x['RootContainer'].containerId === 'undefined'){
                                	x['RootContainer'].containerId = CryptoJS.SHA256(Math.random().toString()).toString();
                                }
                                searchInObject(x['RootContainer']);
                            }
								
                        }
                        searchInObject(com);
						self.designSpaceModels.push(out);
					}
				}
				else{
					var com = avmproject.DesignSpaceModels[m]
					 var out = {
	                            design: com.Name,
	                            cid: com.Name,
	                            componets: []
	                        };
								
	                        var searchInObject = function (x) {
	                            if(typeof x.ComponentInstances != 'undefined'){
	                                for(var i = 0; i < x.ComponentInstances.length; i++){
	                                    out.componets.push({
											id: x.ComponentInstances[i].id.split('{').join('').split('}').join(''),
	                                        type: x.Name,
											typeId: x.id,
	                                        name: x.ComponentInstances[i].Name,
											containerType: x.$type.split('.').pop()
	                                    });
	                                }
	                            }
									
	                            if (Array.isArray(x)) {
	                                for (var i = 0; i < x.length; i++) {
	                                    searchInObject(x[i]);
	                                }
	                            }
	                            if (typeof x['Containers'] != 'undefined') {
	                                searchInObject(x['Containers']);
	                            }
									
	                        }
	                        searchInObject(com);
							self.designSpaceModels.push(out);
				}
            }
        },
        complete: function(){
            console.log("vf-dataproviders: loadDataFiles: complete(projectLocation): "+projectLocation);
            self.loadPending();
        },
        error: function(jqXHR, textStatus, errorThrown){
            alert("Error: Failed to load project file\n\nIf you are running the Project Analyzer locally try using the \"--allow-file-access-from-files\" (without the quotes) option when executing chrome");
            if(jqXHR.statusText == "OK"){jqXHR.statusText = textStatus;}
            self.loadPending();
        }
    }));
}
   
    /**
     *  dataLoadComplete
     *
     *  type: variable
     *
     *  ?.
     */
	this.dataLoadComplete = false;
   
   /**
     *  percentLoaded
     *
     *  type: observable
     *
     *  input: none
     *
     *  output: percentage
     *
     *  returned the ratio of the number of completed Ajax calls to the total number of Ajax calls times 100
     */
   this.percentLoaded = ko.observable(0);
   
    /**
     *  loadPending
     *
     *  type: function
     *
     *  input: none
     *
     *  output: boolean - ?
     *
     *  ?.
     */
	this.loadPending = function(){
        console.log("vf-dataproviders: loadPending: enter");
		
		var completed = 0
		for(var i = 0; i < this.ajaxHandlers().length; i++){
			if(this.ajaxHandlers()[i].state() != 'pending'){
				completed++;
			}
		}
		
		this.percentLoaded((completed / this.ajaxHandlers().length) * 100);
		
		if(this.ajaxHandlers().length < this.numberOfFilesToLoad){
			return true;
		}
		for(var i = 0; i < this.ajaxHandlers().length; i++){
			if(this.ajaxHandlers()[i].state() == 'pending'){
				return true;
			}
		}
		if(this.dataLoadComplete == false){
			this.dataLoadComplete = true;
            console.log("vf-dataprovider: dataLoadComplete: trigger vf-dataLoadComplete event");
			$(document).trigger('vf-dataLoadComplete');
            firstTabLoad();
            this.splash('clear');

            // Generate the dataLoad table
			self.loadSummary([]);
            for (i=0;i<self.numberOfFiles();i++) {
                var info = self.ajaxHandlers()[i];
                var name = self.ajaxURLs()[i];
                var file = i + 1;
                console.log(file+" >>> "+info.statusText+", "+name);
                var statusText = info.statusText;
                if (statusText === 'parsererror') {
                    statusText = 'Parser Error';
                }
				if (statusText === 'success') {
                    statusText = 'OK';
                }
				if (statusText === 'error') {
                    statusText = 'Error';
                }
                self.loadSummary()[i] = {no:i,status:statusText,url:name};
                if (!self.loadError()) {
                    self.loadError(info.statusText === "OK" || info.statusText === "success" ? false : true);
                }
            }
        // Get restored values into PET
            if (state.hasOwnProperty('PETResponses')) {
            console.log("vf-dataprovider: PETResponse.length="+state.PETResponses.length);
            for (i=0; i<state.PETResponses.length; i++) {
                console.log("vf-dataprovider: PETResponse: i="+i);
                t = state.PETResponses[i];
                console.log("vf-dataprovider: PETResponse: name="+t.name);
                if(isSet('self.PET()[state.selectedBenchWithPET][t.name]',{self:self, state:state, t:t})){
                	self.PET()[state.selectedBenchWithPET][t.name].state = t;

	                console.log("vf-dataprovider: PETVariables.length="+state.PETVariables.length);
	                for (j=0; j<state.PETVariables.length; j++) {
	                    console.log("vf-dataprovider: PETVariables: j="+j);
	                    v = state.PETVariables[j];
	                    console.log("vf-dataprovider: PETVariables state: ");
	                    console.log(v);
	                    for (k=0; k<self.PET()[state.selectedBenchWithPET][t.name].inputs.length; k++) {
	                        console.log("v.name="+v.name+
	                                    ", PET label="+self.PET()[state.selectedBenchWithPET][t.name].inputs[k].label);
	                        if (v.name === self.PET()[state.selectedBenchWithPET][t.name].inputs[k].label) {
	                            console.log("set state for "+v.name);
	                            self.PET()[state.selectedBenchWithPET][t.name].inputs[k].state = v;
	                        }
	                    }
	                }
	            }
            }
            
            // Restore the PET selected Test Bench
            self.selectedBenchWithPET(state.selectedBenchWithPET);
            console.log("RESTORE: TB="+self.selectedBenchWithPET());
        }

        //Generate the limits exceeded table
        console.log("vf-dataprovider:  Build the Limits Exceeded Summary Table");
        self.limitSummary([]);
        var elDesigns = [];
        var elTB = [];
        var elIndex = {};
        var design = '';
        var date1 = 0;
        var date2 = 0;
        var key;
        
        // Find latest entry for each design
        for (i=0;i<self.allConfigurations().length;i++) {
            console.log("i="+i);
            console.log("design="+self.allConfigurations()[i].design);
            console.log("time="+self.allConfigurations()[i].time);
            key = self.allConfigurations()[i].designId+"::"+self.allConfigurations()[i].testBench;
            console.log("key="+key);
            if (elIndex.hasOwnProperty(key)) {
                console.log("existing");
                date1 = Date.parse(self.allConfigurations()[i].time.substring(0,13)+":"+
                                   self.allConfigurations()[i].time.substring(14,16)+":"+
                                   self.allConfigurations()[i].time.substring(17));
                date2 = Date.parse(elIndex[key].time.substring(0,13)+":"+
                                   elIndex[key].time.substring(14,16)+":"+
                                   elIndex[key].time.substring(17));
                console.log(date1)
                console.log(date2)
                if (date1 > date2) {
                    elIndex[key].i = i;
                    elIndex[key].time = self.allConfigurations()[i].time;
                    console.log("replace, i="+i);
                }
            } else {
                console.log("new");
                elIndex[key] = {"i":i,"time":self.allConfigurations()[i].time};
            }
        }
        console.log('elIndex');
        console.log(elIndex);
        
        // loop thru configurations() and make an entry for each limit
        for (design in elIndex) {
            i = elIndex[design].i;
            
            // Loop over all variables
            for (j=0;j<self.allConfigurations()[i].variables.length;j++) {
                //console.log("j="+j);
                //console.log(self.allConfigurations()[i].variables[j]);
                if (self.allConfigurations()[i].variables[j].type === 'limit') {
                    //console.log("push");
                    self.limitSummary().push({design: self.allConfigurations()[i].design,
                                              testBench: self.allConfigurations()[i].variables[j].testBench,
                                              ActualValue: self.allConfigurations()[i].variables[j].ActualValue,
                                              Value: self.allConfigurations()[i].variables[j].Value,
                                              LimitName: self.allConfigurations()[i].variables[j].LimitName,
                                              LimitFullPath: self.allConfigurations()[i].variables[j].LimitFullPath,
                                              instance: self.allConfigurations()[i].variables[j].instance,
                                              time: self.allConfigurations()[i].time,
                                              designId: self.allConfigurations()[i].designId
                                              });
                    
                    // Add to design and test bench lists
                    if (findStringInArray(elDesigns,self.allConfigurations()[i].design) === -1) {
                        elDesigns.push(self.allConfigurations()[i].design);
                    }
                    //console.log(elDesigns);
                    if (findStringInArray(elTB,self.allConfigurations()[i].variables[j].testBench) === -1) {
                        elTB.push(self.allConfigurations()[i].variables[j].testBench);
                    }
                    //console.log(elTB);
                }
            }
        }
        self.numberOfLimitExceededDesigns(elDesigns.length);
        self.numberOfLimitExceededTestBenches(elTB.length);
        if (self.numberOfLimitExceededDesigns() + self.numberOfLimitExceededTestBenches()) {
            self.limitError(true);
        }
		}
		return false;
	}
   
    this.vmState = function() {
        console.log("vf-dataprovider: vmState: enter");
        dumpVM();
    }

    this.clearAndReset = function(){
    	localStorage.clear();
    	location.reload();
    }
	
	this.showDsaWidgets = ko.computed(function () {
            return (this.currentVariables().length >= 2 && this.currentConfigurations().length >= 1);
	}, this).extend({
		throttle : 100
	});
	
	
    /**
     *  dataproviders
     *
     *  type: object
     *
     *  This object contains a set of computed variables (arrays).
     */
	this.dataproviders = {
		parent : this,
		
		/**
        *  constraintplot
        *
        *  type: observable
        *
        *  ?.
        */
		constraintplot : ko.computed(function(){
			console.log("vf-dataprovider: dataproviders.constraintplot: enter");
			
			var TB = this.selectedBenchWithPET();
			var out = {};
			
			out.fidelity = this.selectedFidelity();
			// setup user inputs
			out.userInputs = {};
			out.userInputs.selectedResponses = [];
			if (isSet("self.listOfPETResponses", {self:this}))
			{
				for(var i = 0; i < this.listOfPETResponses().length; i++)
				{
					if(vm.listOfPETResponses()[i].display())
					{
						out.userInputs.selectedResponses.push({
							'rid' : this.listOfPETResponses()[i].rid,
							'name' : this.listOfPETResponses()[i].name(),
							'value' : this.listOfPETResponses()[i].value(),
							'inequality' : this.listOfPETResponses()[i].greaterThan() === "true" ? '>=' : '<=',
							'color' : this.listOfPETResponses()[i].color()
						});
					}
				}
			}
			
			out.userInputs.selectedVariables = [];
			if (isSet("self.listOfPETVariables", {self:this}))
			{
				for(var i = 0; i < this.listOfPETVariables().length; i++)
				{
					if(vm.listOfPETVariables()[i].display())
					{
						out.userInputs.selectedVariables.push(vm.listOfPETVariables()[i].name());
					}
				}
			}
			
			out.userInputs.defaultVariableValues = [];
			if (isSet("self.listOfPETVariables", {self:this}))
			{
				for(var i = 0; i < this.listOfPETVariables().length; i++)
				{
					out.userInputs.defaultVariableValues.push({
						'label' : vm.listOfPETVariables()[i].name(),
						'defaultValue' : vm.listOfPETVariables()[i].value()
					});
				}
			}
			
			// pass response data
			out.responses = [];
			if (isSet("self.listOfPETResponses", {self:this}))
			{
				for(var i = 0; i < this.listOfPETResponses().length; i++)
				{
					if(vm.listOfPETResponses()[i].display())
					{
						out.responses.push(self.PET()[TB][vm.listOfPETResponses()[i].name()]);
					}
				}
			}
			
			console.log(out);
			return out;
		}, this).extend({
			throttle : 100
		}),
		selectedConfigurationDetails : ko.computed(function(){
		
		// last selected configuration
		var out = {};
		out.Details = [];
		
		//dummy data
		//tempGroupBody = [];
		//tempGroupBody.push("annotate1");
		//tempGroupBody.push("annotate2");
	
		
			var lsc = this.configurations()[this.listOfConfigurations().indexOf(this.selectedConfiguration())];
			if(typeof lsc != 'undefined'){
				console.debug(lsc);
			
				out.DesignId = lsc.designId,
				out.Time = lsc.time
				//out.Details.push({'GroupTitle' : "Annotates", 'GroupBody': lsc.annotate });
				//out.Details.push({'GroupTitle' : "Annotations", 'GroupBody':tempGroupBody }); //with dummy data
				
			}
			return out;
			}, this).extend({
			throttle : 100
		}),
		
		
		/**
        *  profiler
        *
        *  type: observable
        *
        *  ?.
        */
		profiler : ko.computed(function(){
			console.log("vf-dataprovider: dataproviders.profiler: enter");
			
			var out = {};
			
			out.Inputs = [];
			out.Outputs = [];
			
			var TB = this.selectedBenchWithPET();
			
			if (isSet("self.listOfPETVariables", {self:this}))
			{
				for(var i = 0; i < this.listOfPETVariables().length; i++)
				{
					//if(vm.listOfPETVariables()[i].display())
					//{
						out.Inputs.push({'name' : vm.listOfPETVariables()[i].name(), 'value': vm.listOfPETVariables()[i].value(), 'max': vm.listOfPETVariables()[i].max, 'min': vm.listOfPETVariables()[i].min, 'display':vm.listOfPETVariables()[i].display()});
					//}
				}
			}
			
			if (isSet("self.listOfPETResponses", {self:this}))
			{
				for(var i = 0; i < this.listOfPETResponses().length; i++)
				{
					if(vm.listOfPETResponses()[i].display())
					{
						out.Outputs.push({'name' : vm.listOfPETResponses()[i].name(), 'max': vm.listOfPETResponses()[i].max, 'min': vm.listOfPETResponses()[i].min, 'function' : vm.PET()[TB][vm.listOfPETResponses()[i].name()].function});
					}
				}
			}

			
			
			console.log(out);
			return out;
		}, this),
		
        /**
        *  parallelAxis
        *
        *  type: computed array
        *
        *  ?.
        */
		parallelAxis : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.parallelAxis: enter");
			
				var out = {};
				out.design = [];
				out.variables = [];
				out.labels = [];
				out.testBenchs = [];
				out.cid = []
				out.Unit = [];
				out.type = [];
				out.violations = [];
				out.color = [];
				
				for (var i = 0; i < this.currentVariables().length; i++) {
					out.variables.push(this.currentVariables()[i].testBench + "_" + this.currentVariables()[i].Name);
					out.labels.push(this.currentVariables()[i].Name);
					out.Unit.push(this.currentVariables()[i].Unit);
					out.type.push(this.currentVariables()[i].type);
					out.testBenchs.push(this.currentVariables()[i].testBench);
				}
				out.thresholds = this.thresholds();
				out.objectives = this.objectives();
				for (var i = 0; i < this.currentConfigurations().length; i++) {
					out.design.push(this.currentConfigurations()[i].design);
					out.cid.push(this.currentConfigurations()[i].id);
					out.color.push(this.currentConfigurations()[i].color());
				}
				out.values = [];
				for (var i = 0; i < this.currentConfigurations().length; i++) {
					out.values.push({
						design : this.currentConfigurations()[i].design,
						cid : this.currentConfigurations()[i].id
					});
					out.violations.push(this.isInViolation(this.currentConfigurations()[i].design));
					for (var j = 0; j < this.currentConfigurations()[i].variables.length; j++) {
						if (this.variableInArray(this.currentVariables(), {
								Name : this.currentConfigurations()[i].variables[j].Name,
								testBench : this.currentConfigurations()[i].variables[j].testBench
							})) {
							if(sciNotation(this.currentConfigurations()[i].variables[j].Value).expValue > -19){
								out.values[out.values.length - 1][this.currentConfigurations()[i].variables[j].testBench + "_" + this.currentConfigurations()[i].variables[j].Name] = +this.currentConfigurations()[i].variables[j].Value;
							}
							else{
								out.values[out.values.length - 1][this.currentConfigurations()[i].variables[j].testBench + "_" + this.currentConfigurations()[i].variables[j].Name] = 0;
							}
						}
					}
				}
                console.log(out);
				return out;
		}, this).extend({
			throttle : 100
		}),
   
        /**
        *  multiVariate
        *
        *  type: computed array
        *
        *  ?.
        */
		multiVariate : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.multiVariate: enter");
			
				var data = {};
				data.variables = [];
				data.testBenchs = [];
				data.labels = [];
				data.Unit = [];
				data.type = [];
				data.color = [];
				for (var i = 0; i < this.currentVariables().length; i++) {
					data.variables.push(this.currentVariables()[i].testBench + "_" + this.currentVariables()[i].Name);
					data.labels.push(this.currentVariables()[i].Name);
					data.Unit.push(this.currentVariables()[i].Unit);
					data.type.push(this.currentVariables()[i].type);
					data.testBenchs.push(this.currentVariables()[i].testBench);
				}
				data.design = [];
				data.values = [];
				data.cid = [];
				data.violations = [];
				data.objectives = this.objectives();
				data.thresholds = this.thresholds();
				for (var i = 0; i < this.currentConfigurations().length; i++) {
					data.design.push(this.currentConfigurations()[i].design);
					data.values.push({
						design : this.currentConfigurations()[i].design,
						cid : this.currentConfigurations()[i].id
					});
					data.color.push(this.currentConfigurations()[i].color());
					data.cid.push(this.currentConfigurations()[i].id);
					data.violations.push(this.isInViolation(this.currentConfigurations()[i].design));
					for (var j = 0; j < this.currentConfigurations()[i].variables.length; j++) {
						if (this.variableInArray(this.currentVariables(), {
								Name : this.currentConfigurations()[i].variables[j].Name,
								testBench : this.currentConfigurations()[i].variables[j].testBench
							})) {
							if(sciNotation(this.currentConfigurations()[i].variables[j].Value).expValue > -19){
								data.values[data.values.length - 1][this.currentConfigurations()[i].variables[j].testBench + "_" + this.currentConfigurations()[i].variables[j].Name] = +this.currentConfigurations()[i].variables[j].Value;
							}
							else{
								data.values[data.values.length - 1][this.currentConfigurations()[i].variables[j].testBench + "_" + this.currentConfigurations()[i].variables[j].Name] = 0;
							}
						}
					}
				}
				return data;
		}, this).extend({
			throttle : 100
		}),
   
        /**
        *  pareto
        *
        *  type: computed array
        *
        *  ?.
        */
		pareto : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.pareto: enter");
			var out = {
				data: [],
				labels: {
					x: this.pfXaxis(),
					y: this.pfYaxis()
				},
				units: {
					x: '',
					y: ''
				}
			};
			
			for(var i = 0; i < this.scores().history.length; i++){
				out.data.push({
					x: this.scores().history[i][this.pfXaxis()],
					y: this.scores().history[i][this.pfYaxis()],
					cid: this.scores().history[i]['design'],
					design: this.scores().history[i]['design'],
					total: 0
				});
				for(var j = 0; j < this.scores().categories.length; j++){
					out.data[i].total += this.scores().history[i][this.scores().categories[j]];
				}
			}
			//$(document).trigger('vf-selectionChange');
			return out;
		}, this).extend({
			throttle : 100
		}),
		
		
///////////////////////////////mw//////////////////////////////////////////
	
		qrHeatMap: ko.computed(function() {
			console.log("vf-qrheatmap: dataproviders.profiler: enter");
	
			var out = {};				
			var values = {};
			var valuess= [];
			out["benches"] = []; //x
			out["configs"] = []; //y
		
		
			//find unique x axis names (requirement)
			if(isSet('that.formalverification().length',{that:this})){
				for (var i = 0; i < this.formalverification().length; i++ ){
					if ( out["benches"].indexOf(this.formalverification()[i].ReqName) == -1)
						out["benches"].push(this.formalverification()[i].ReqName);
					if ( out["configs"].indexOf(this.formalverification()[i].DesignName) == -1)
						out["configs"].push(this.formalverification()[i].DesignName);	
					values["benches"] = this.formalverification()[i].ReqName;
					values["benchesX"] = out["benches"].indexOf(this.formalverification()[i].ReqName);
					values["configs"] = this.formalverification()[i].DesignName;
					values["configsY"] = out["configs"].indexOf(this.formalverification()[i].DesignName);
					values["values"] = this.formalverification()[i].Result; //SUCCESS, FAIL or UNKNOWN
					valuess.push(values);
					values = {};
				}
			}
			
			out["values"] = valuess;
			return out;
			
		}, this),
///////////////////////////////////////////// end mw///////////////////////////////////////////////////////		
		pccHeatMap: ko.computed(function() {
			console.log("vf-heatmap: dataproviders.pccHeatMap: enter");
			
			var out = {};		
			out["benches"] = [];
			out["configs"] = [];

 			var data = {};
			var dataArray = [];
			var val = 0;
			
			for (var i = 0; i < this.activePccResults().length; i++)
			{
				if (jQuery.inArray(this.activePccResults()[i].testBench, out["benches"]) == -1)
				{
					out["benches"].push(this.activePccResults()[i].testBench);
				}
				
				if (jQuery.inArray(this.activePccResults()[i].design, out["configs"]) == -1)
				{
					out["configs"].push(this.activePccResults()[i].design);
				}
			}
			
			if (this.activePccResults().length > 0)
			{
				for (var i = 0; i < out["configs"].length; i++)
				{
					for (var j = 0; j < out["benches"].length; j++)
					{
						for (var k = 0; k < this.activePccResults().length; k++)
						{
							if (out["benches"][j] == this.activePccResults()[k].testBench && out["configs"][i] == this.activePccResults()[k].design)
							{
								val = -1;
								if (this.activePccResults()[k].distExist)
								{
									//var method = this.dataproviders.pccJointPccMethodsTBvsConfig(out["benches"][j], out["configs"][i])[self.activeUpPccResultsIds()[k]];
									var methods = this.dataproviders.pccJointPccMethodsTBvsConfig(out["benches"][j], out["configs"][i]);
									//var method = methods[self.activeUpPccResultsIds()[k]];
									var method = methods[0];
									
									val = this.activePccResults()[k].PCCResults.byMethod[method].JointPCC;
								}
								else if (!this.activePccResults()[k].sensExist)
								{
									val = -1; // -1 flag means there is no PCC data for this config & TB
								}
								
								data = {
									benches : out["benches"][j],
									benchesX : j,
									configs : out["configs"][i],
									configsY: i,
									values : val
								};
								
								dataArray.push(data);
								data = {};
							}
						}
					}
				}
			}
			
			out["values"] = dataArray;
			
			// sorts the heatmap by descending pcc in the column with the highest pcc
			function heatmaphelper(data) {
				var temp = data.values;
				temp.sort(function(a,b){return b.values-a.values});
			
				var newBenches = [];
				var newConfigs = [];
				for(var i=0; i<temp.length; i++) {
					if(newBenches.indexOf(temp[i].benches) == -1) {
						newBenches.push(temp[i].benches);
					}
					if(newConfigs.indexOf(temp[i].configs) == -1) {
						newConfigs.push(temp[i].configs);
					}
				}
				var newValues = [];
				for(var i=0; i<newConfigs.length; i++) {
					for(var j=0; j<newBenches.length; j++) {
						for(var k=0; k<data.values.length; k++) {
							if(data.values[k].configs == newConfigs[i] && data.values[k].benches == newBenches[j]) {
								data.values[k].benchesX = j;
								data.values[k].configsY = i;
								newValues.push(data.values[k]);
							}
						}
					}
				}
				
				var newOut = [];
				console.log("TESTING2");
				console.log(data.benches);
				console.log(newBenches);
				console.log(data.configs);
				console.log(newConfigs);
				data.benches = newBenches;
				data.configs = newConfigs;
				data.values = newValues;
				
				return data;				
			}
			
			function sorter(data) {
				var temp = [];
				var temp2 = [];
				
				for(var i=0; i<data.values.length; i++) {
					if(data.values[i].benchesX == 0) {
						temp.push(data.values[i]);
					} else {
						temp2.push(data.values[i]);
					}					
				}
				temp.sort(function(a,b){return b.values-a.values});
				temp2.sort(function(a,b){return b.values-a.values});

				var newBenches = [];
				var newConfigs = [];
				temp = temp.concat(temp2);
				for(var i=0; i<temp.length; i++) {
					if(newBenches.indexOf(temp[i].benches) == -1) {
						newBenches.push(temp[i].benches);
					}
					if(newConfigs.indexOf(temp[i].configs) == -1) {
						newConfigs.push(temp[i].configs);
					}
				}
				
				var newValues = [];
				for(var i=0; i<newConfigs.length; i++) {
					for(var j=0; j<newBenches.length; j++) {
						for(var k=0; k<data.values.length; k++) {
							if(data.values[k].configs == newConfigs[i] && data.values[k].benches == newBenches[j]) {
								data.values[k].benchesX = j;
								data.values[k].configsY = i;
								newValues.push(data.values[k]);
							}
						}
					}
				}
				
				console.log("TESTING3");
				console.log(data.benches);
				console.log(newBenches);
				console.log(data.configs);
				console.log(newConfigs);
				data.benches = newBenches;
				data.configs = newConfigs;
				data.values = newValues;
				data.benches = newBenches;
				data.configs = newConfigs;
				data.values = newValues;
				
				
					
				return data;
			}
			
			
			function heatmapSorter(data) {
				var newBenches = [];
				var newConfigs = [];
				var columnTotalPCC = Array.apply(null, new Array(data.benches.length)).map(Number.prototype.valueOf,0);
				for(var i=0; i<data.values.length; i++) {
					var loc = data.benches.indexOf(data.values[i].benches)
					columnTotalPCC[loc] = columnTotalPCC[loc] + data.values[i].values
				}
				// sort the benches into descending order of total PCC
				var temp = [];
				for(var j=0; j<data.benches.length; j++) {
					temp.push({"bench": data.benches[j], "value":columnTotalPCC[j]})
					console.log('infloop 1')
				}
				temp.sort(function(a, b) {
					return ((a.value > b.value) ? -1 : ((a.value == b.value) ? 0 : 1));
				});
				for (var k=0; k<temp.length; k++) {
					newBenches[k] = temp[k].bench;
					console.log('infloop 2')
				}
				// sort the configurations in descending order of PCC
				var temp2 = data.values;
				temp2.sort(function(a, b) {
					return ((a.values > b.values) ? -1 : ((a.values == b.values) ? 0 : 1));
				});	
				for(var i=0; i<temp2.length; i++) {
					if(temp2[i].benches == newBenches[0]) {
						newConfigs.push(temp2[i].configs)
					} 
					console.log('infloop 3')
				}
				var tempExtra = [];
				for(var i=0; i<temp2.length; i++) {
					if(temp2[i].benches != newBenches[0]) {
						if(newConfigs.indexOf(temp2[i].configs) == -1 && tempExtra.indexOf(temp2[i].configs) == -1) {
							tempExtra.push(temp2[i].configs)
						}
					}
					console.log('infloop 4')
				}
				newConfigs = newConfigs.concat(tempExtra)
				
				// updates the x&y position of the values
				var newValues = [];
				for(var i=0; i<data.values.length; i++) {
					var temp = data.values[i];
					newValues.push(data.values[i]);
					newValues[newValues.length-1].benchesX = newBenches.indexOf(data.values[i].benches)
					newValues[newValues.length-1].configsY = newConfigs.indexOf(data.values[i].configs)
				}
				
				
				console.log(newValues)
				
				data.benches = newBenches;
				data.configs = newConfigs;
				data.values = newValues;
			}
			
			//var temp = heatmaphelper(out);
			//var temp = sorter(out);
			//var temp = heatmapSorter(out);
			//var temp = out
			//var temp = sorter(out);
			return out;			
		}, this).extend({
			throttle : 100
		}),
		
		/**
        *  refreshPCCheatMap
        *
        *  type: computed array
        *
        *  This forced the heatmap to refresh
        */
		refreshPCCheatMap : ko.computed(function() {
			console.log("vf-heatmap: dataproviders.refreshPCCheatMap: enter");
			
			return this.activePccResults();
			
		}, this).extend({
			throttle : 100
		}),
		
        /**
        *  rankingsHistory
        *
        *  type: computed array
        *
        *  ?.
        */
		rankingsHistory : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.rankingsHistory: enter");
			var data = [];
			var designs = [];
			for (var i = 0; i < this.scores().categories.length; i++) {
				data.push([]);
			}
			for (var j = 0; j < this.scores().history.length; j++) {
				if(this.scores().history[j].design !== null){ // trying to filter out nulls from vf
					designs.push(this.getConfigurationsNameById('CID' + this.scores().history[j].design));
				}
				for (var k = 0; k < this.scores().categories.length; k++) {
					if(this.scores().history[j].design !== null){ // trying to filter out nulls from vf
						data[k].push({
							x : this.scores().history[j].design,
							y : this.scores().history[j].scoring[this.scores().categories[k]],//["Aggregated Score"],  //[this.scores().categories[k]],
							i : j
						});
					}
				}
			}
			out = {
				"data" : data,
				"labels" : this.scores().categories,
				//"labels" : ["Score"],
				"designs" : designs,
				"xLabel" : "Designs"
			};
			return out;
		}, this).extend({
			throttle : 100
		}),
   
        /**
        *  leaderBoardRankings
        *
        *  type: computed array
        *
        *  ?.
        */
		leaderBoardRankings : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.leaderBoardRankings: enter");
			var out = {
				headers : ['Teams'],
				data : []
			};
			for (var i = 0; i < this.scores().types.length; i++) {
				out.headers.push(this.scores().types[i]);
			}
			for (var j = 0; j < this.scores().teams.length; j++) {
				out.data.push(['<a href="'+this.scores().teamsURLs[j]+'" target="_blank">'+this.scores().teamsNames[j]+'</a>']);
				for (var i = 0; i < this.scores().types.length; i++) {
					if(isSet('self.scores().rankings[self.scores().types[i]][self.scores().teams[j]]',{j:j, i:i, self:this})){
					    out.data[j].push(this.scores().rankings[this.scores().types[i]][this.scores().teams[j]]);
					}
					else{
					    out.data[j].push('');
					}
				}
			}
			return out;
		}, this).extend({
			throttle : 100
		}),
		
		/**
        *  fileLoadSummary
        *
        *  type: computed array
        *
        *  ?.
        */
		fileLoadSummary : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.fileLoadSummary: enter");
			var out = {
				headers : ['File Number', 'URL', 'Status'],
				data : []
			};
			for (var j = 0; j < this.loadSummary().length; j++) {
				out.data.push([
					this.loadSummary()[j].no,
					'<a href="'+this.loadSummary()[j].url+'" target="_blank">' + this.loadSummary()[j].url + '</a>',
					this.loadSummary()[j].status
				]);
			}
			return out;
		}, this).extend({
			throttle : 100
		}),
		
		/**
        *  limitExceededSummary
        *
        *  type: computed array
        *
        *  ?.
        */
		limitExceededSummary : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.fileLoadSummary: enter");
			var out = {
				headers : ['Test Bench', 'Instance', 'Limit Name', 'Design (Design Id)', 'Limit Value', 'Actual Value'],
				data : []
			};
			for (var j = 0; j < this.limitSummary().length; j++) {
				out.data.push([
					this.limitSummary()[j].testBench,
					this.limitSummary()[j].instance,
					'<span title="'+this.limitSummary()[j].LimitFullPath+'">'+this.limitSummary()[j].LimitName+'</span>',
					this.limitSummary()[j].design + " #" + this.limitSummary()[j].designId.slice(-4),
					this.limitSummary()[j].Value,
					this.limitSummary()[j].ActualValue
				]);
			}
			return out;
		}, this).extend({
			throttle : 100
		}),
   
        /**
        *  designInfoBox
        *
        *  type: computed array
        *
        *  ?.
        */
		designInfoBox : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.designInfoBox: enter");
			var out = {
				headers : ['Variables', 'Test Bench', 'Values', 'Units'],
				data : []
			};
			
			// last selected configuration
			var lsc = this.configurations()[this.listOfConfigurations().indexOf(this.selectedConfiguration())];
			if(typeof lsc != 'undefined'){
				for(var i = 0; i < lsc.variables.length; i++){
					if(lsc.variables[i].type !== "limit"){
						out.data.push([
							'<a href="'+lsc.variables[i].location+'" target="_blank">' + lsc.variables[i].Name + '</a>',
							lsc.variables[i].testBench,
							lsc.variables[i].Value,
							lsc.variables[i].Unit
						]);
					}
				}
			}
			return out;
		}, this).extend({
			throttle : 100
		}),
		
		/**
        *  Comparer Design Info Table
        *
        *  type: computed array
        *
        *  ?.
        */
		comparerGeneralInfo : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.comparerGeneralInfo: enter");
			
			var data = clone(this.listOfCompareConfigurations);
			
			var out = {
				headers : ["Variable", "Testbench", "units"],
				data : []
			};
			
			var configVarandTBs = [];
			var varNameTBConcat = [];
			var varNameTBConcatUnique = [];
			var NameOfCurrentVarAndTB = "";
			
			for (var i = 0; i < data.length; i++)
			{
				if(data[i] != " ")
				{
					out.headers.splice(-1, 0, data[i] + ' <a href="#" config="' + data[i] + '" onclick="vm.removeDesignFromCompare(\'' + data[i] + '\')"><i class="icon-remove-sign icon-white" title="Click to remove this design from the Comparison table."></i></a>');
				}
			}
			
			var row = [];
			
			//variable and testbench loops.  If a combination of variable and testbenches are unique, add them.
			for (var i = 0; i < data.length; i++)
			{
				if (data[i] != " ") //loop through configs selected
				{
					for (var j = 0; j < this.configurations().length; j++) //go through every config possible
					{
						if(this.configurations()[j].design == data[i]) //does the current config's name equal the name of the selected config?
						{
							for (var k = 0; k < this.configurations()[j].variables.length; k++) //go through the config's variables
							{
								NameOfCurrentVarAndTB = this.configurations()[j].variables[k].Name + "_" + this.configurations()[j].variables[k].testBench;
								varNameTBConcat.push(NameOfCurrentVarAndTB);
								
								if (jQuery.inArray(NameOfCurrentVarAndTB, varNameTBConcatUnique) == -1)
								{
									if (this.configurations()[j].variables[k].Name != undefined) //if the name is undefined, then it is a limit violation
									{
										varNameTBConcatUnique.push(NameOfCurrentVarAndTB);
										configVarandTBs.push({
											//config: this.configurations()[j].design,
											varName: this.configurations()[j].variables[k].Name, 
											testbench: this.configurations()[j].variables[k].testBench,
											varNameTBConcat: varNameTBConcatUnique[varNameTBConcatUnique.length - 1],
											units: this.configurations()[j].variables[k].Unit
										});
										
										out.data.push([this.configurations()[j].variables[k].Name, this.configurations()[j].variables[k].testBench, this.configurations()[j].variables[k].Unit]);
										
										for (var l = 0; l < data.length; l++)
										{
											out.data[out.data.length - 1].splice(-1, 0, "");
										}
									}
								}
							}
						}
					}
				}
			}
			
			var count = 1;
			var location;
			var h;
			
			for (var i = 0; i < configVarandTBs.length; i++)
			{	
				if (data[i] != " ") //loop through configs selected
				{
					for (var j = 0; j < this.configurations().length; j++)
					{
						h = jQuery.inArray(this.configurations()[j].design, data);	
						if (h != -1)
						{
							for (var k = 0; k < this.configurations()[j].variables.length; k++)
							{

								NameOfCurrentVarAndTB = this.configurations()[j].variables[k].Name + "_" + this.configurations()[j].variables[k].testBench;
								if (this.configurations()[j].variables[k].Name != undefined) //if the name is undefined, then it is a limit violation
								{
									location = jQuery.inArray(NameOfCurrentVarAndTB, varNameTBConcatUnique);
									if (location != -1) //if the current var abd TB combo is in the unique collection, then add them to the dataset!
									{
										out.data[location][h+2] = this.configurations()[j].variables[k].Value;
										//out.data[location].splice(-1, 0, this.configurations()[j].variables[k].Value);
									}
								}
							}
						}
					}
				}
			}
			
			return out;
		}, this).extend({
			throttle : 100
		}),
			
        /**
        *  dsaTable
        *
        *  type: computed array
        *
        *  ?.
        */
		dsaTable : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.dsaTable: enter");
			var data = {};
				data.headers = ['Designs'];
				data.data = [];
				for (var j = 0; j < this.currentVariables().length; j++) {
					data.headers.push(this.currentVariables()[j].Name);
				}
				for(var i = 0; i < this.currentConfigurations().length; i++){
					data.data.push([this.currentConfigurations()[i].design]);
					for (var j = 0; j < this.currentVariables().length; j++) {
						for (var k = 0; k < this.currentConfigurations()[i].variables.length; k++) {
							if(this.currentVariables()[j].Name == this.currentConfigurations()[i].variables[k].Name){
								data.data[i].push(this.currentConfigurations()[i].variables[k].Value);
							}
						}
					}
				}
				return data;
		}, this).extend({
			throttle : 100
		}),
   

		 /**
        *  cfgVis
        *
        *  type: computed array
        *
        *  ?.
        */
		cfgVis : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.cfgVis: enter");
			var groupVar = this.dsVisSelected();
			var colorVar = this.dsVisColor();
			var out = {
				headers : ['Designs'],
				data : [],
				groupVar: [groupVar],
				colorVar: [colorVar]
			};
			
			// Collect and sort all the typeId's and headers
			var typeIds = [];
			for(var i = 0; i < this.designSpaceModels().length; i++){
				for(var j = 0; j < this.designSpaceModels()[i].componets.length; j++){
					if(!typeIds.inArray(this.designSpaceModels()[i].componets[j].type + this.designSpaceModels()[i].componets[j].typeId)){
						typeIds.push(this.designSpaceModels()[i].componets[j].type + this.designSpaceModels()[i].componets[j].typeId);
						out.headers.push(this.designSpaceModels()[i].componets[j].type + ' (' + this.designSpaceModels()[i].componets[j].containerType + ')');
					}
				}
			}
			
			// create an array[row][col][entry] for each cell in the table
			var data = [];
			for(var i = 0; i < this.listOfConfigurations().length; i++){
				// create [row] for configuration
				data.push([]);
				
				// find configuration in componets()
				for(var j = 0; j < this.componets().length; j++){
					// match based on design name
					if(this.listOfConfigurations()[i].split(' #')[0] === this.componets()[j].design){
						for(var k = 0; k < typeIds.length; k++){
							// create [col] and [entry] for configuration
							data[i].push([]);
							for(var l = 0; l < this.componets()[j].componets.length; l++){
								if(typeIds[k] === (this.componets()[j].componets[l].type + this.componets()[j].componets[l].typeId)){
									data[i][k].push(this.componets()[j].componets[l].name);
								}
							}
						}
					}
				}
			}
			
			// generate out.data based on data[row][col][entry]
			for(var i = 0; i < this.listOfConfigurations().length; i++){
				// the first col is the design name
				out.data.push([this.listOfConfigurations()[i]]);
				
				
				for(var j = 0; j < typeIds.length; j++){
					if(isSet('data[i][j]',{data:data,i:i,j:j})){
						data[i][j].sort();
						if(data[i][j].length < 2){
							if(data[i][j][0] === undefined){
								out.data[i].push("n/a");
							}
							else{
								out.data[i].push(data[i][j][0]);
							}
						}
						else{
							out.data[i].push("compound");
						}
					}
					else{
						out.data[i].push("n/a");
					}
				}
			}
			
			
			var comps = [];
			
			//calculates the position of the compounds
			for(var i = 0; i< out.headers.length; i++) {
				if(out.headers[i].indexOf("Compound") != -1) {
					comps.push(i);
				}
			}
			comps.reverse();
			
			//removes compounds from data
			for(var i = 0; i<out.data.length; i++) {
				for(var j = 0; j<comps.length; j++) {
					out.data[i].splice(comps[j],1);
				}
			}
		
		var data2 = []
			for (var i = 0; i < out.data.length; i++) {
				// data2[i] = [{rad: 20, config: "test"}, data2[i]];
				data2[i] = {
					rad: 20,
					config: out.data[i][0],
					comps: out.data[i].slice(1, out.data[i].length),
					color: 20
				};
			}
			out.data = data2;
			
			var heads = [];
			for(var i = 0; i<out.headers.length; i++) {
				var count = 0;
				if(out.headers[i].indexOf("Compound") == -1){
					for(var j = 0; j<data.length; j++) {
						if(data[j][i] != ""){
							count = count + 1;
						}
					}
					if (count>0){
						heads.push(out.headers[i]);
					}
				}
			}
			out.headers = heads.slice(1,heads.length);
			
			return out;
		}, this).extend({
			throttle : 100
		}),
   
		/**
        *  cfgTable
        *
        *  type: computed array
        *
        *  ?.
        */
		cfgTable : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.cfgTable: enter");
			var out = {
				headers : ['Designs'],
				data : []
			};
			
			
			// Collect and sort all the typeId's and headers
			var typeIds = [];
			for(var i = 0; i < this.designSpaceModels().length; i++){
				for(var j = 0; j < this.designSpaceModels()[i].componets.length; j++){
					if(!typeIds.inArray(this.designSpaceModels()[i].componets[j].type + this.designSpaceModels()[i].componets[j].typeId)){
						typeIds.push(this.designSpaceModels()[i].componets[j].type + this.designSpaceModels()[i].componets[j].typeId);
						out.headers.push(this.designSpaceModels()[i].componets[j].type + ' (' + this.designSpaceModels()[i].componets[j].containerType + ')');
					}
				}
			}
			//typeIds.sort();
			
			// create an array[row][col][entry] for each cell in the table
			var data = [];
			for(var i = 0; i < this.listOfConfigurations().length; i++){
				// create [row] for configuration
				data.push([]);
				
				// find configuration in componets()
				for(var j = 0; j < this.componets().length; j++){
					// match based on design name
					if(this.listOfConfigurations()[i].split(' #')[0] === this.componets()[j].design){  //if(this.listOfConfigurations()[i] === this.componets()[j].design){
						for(var k = 0; k < typeIds.length; k++){
							// create [col] and [entry] for configuration
							data[i].push([]);
							for(var l = 0; l < this.componets()[j].componets.length; l++){
								if(typeIds[k] === (this.componets()[j].componets[l].type + this.componets()[j].componets[l].typeId)){
									data[i][k].push(this.componets()[j].componets[l].name);
								}
							}
						}
					}
				}
			}
			
			// generate out.data based on data[row][col][entry]
			for(var i = 0; i < this.listOfConfigurations().length; i++){
				// the first col is the design name
				out.data.push([this.listOfConfigurations()[i]]);
				
				for(var j = 0; j < typeIds.length; j++){
					if(isSet('data[i][j]',{data:data,i:i,j:j})){
						data[i][j].sort();
						if(data[i][j].length < 2){
							if(data[i][j][0] === undefined){
								out.data[i].push("");
							}
							else{
								out.data[i].push(data[i][j][0]);
							}
						}
						else{
							var html = '<button onclick="$(this).hide()" type="button" class="btn btn-inverse" data-toggle="collapse" data-target="#moreBody'+i.toString()+j.toString()+'">More</button><div id="moreBody'+i.toString()+j.toString()+'" class="collapse"><table style="color:white" onclick="$(this).parent().prev().click().show()"><tbody style="border-width: 1px;border: 1px solid #888;-webkit-border-radius: 10px;-moz-border-radius: 10px;border-radius: 10px;background-color: #111;">';
							for(var k = 0; k < data[i][j].length; k++){
								html += '<tr><td style="border-width: 0px;">' + data[i][j][k] + '</td></tr>';
							}
							html += '</tbody></table></div>';
							out.data[i].push(html);
						}
					}
					else{
						out.data[i].push("");
					}
				}
			}
			return out;
		}, this).extend({
			throttle : 100
		}),
		/**
        *  DesignDetailsArtifacts type: array
        *
        */
		designArtifacts : ko.computed(function () {
            console.log("vf-designArtifacts");
			var lsc = this.configurations()[this.listOfConfigurations().indexOf(this.selectedConfiguration())];
			if(typeof lsc != 'undefined')
				return lsc.visualizationArtifacts;
			else	
				return null;
			
		}, this).extend({
			throttle : 100
		}),
		
        /**
        *  requirementsAnalysis
        *
        *  type: computed array
        *
        *  ?.
        */
		requirementsAnalysis : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.requirementsAnalysis: enter");
			return this.requirementsStructure();
		}, this).extend({
			throttle : 100
		}),
		
		/**
        *  requirementsList
        *
        *  type: computed array
        *
        *  ?.
        */
		requirementsList : ko.computed(function () {
            console.log("vf-dataprovider: dataproviders.requirementsList: enter");
			var out = {
				headers : ['Requirement', 'Test Bench', 'Priority', 'Threshold', 'Objective', 'Value', 'Units'],
				data : []
			};
			
			for(var i = 0; i < this.requirements().length; i++){
				var a = [
					this.requirements()[i].name,
					this.requirements()[i].testBench,
					this.requirements()[i].Priority,
					this.requirements()[i].threshold,
					this.requirements()[i].objective,
					this.requirements()[i].currentVal,
					this.requirements()[i].unit
				];
				
				out.data.push(a.undefined2Blank());
			}
			
			return out;
		}, this).extend({
			throttle : 100
		}),


		/**
        *  configSettingWidget
        *
        *  Use by the DSA settings widget
        */
		configSettingWidget : function () {
            var out = {
            	"dataRight" : [],
            	"dataLeft" : []
            };

            var variableArray = [];
            var designArray = [];

			
			//id must be unique among all data (variable + design)
			var designIds = {};
			for(var i = 0; i < self.configurations().length; i++){
				designIds[self.configurations()[i].design] = "d" + i;
			}
			var variableIds = {};
        	for(var i = 0; i < self.variables().length; i++){
        		var varObj = {
					"id" : "v"+ i,
        			"displayName"	: self.variables()[i].Name + " [" + self.variables()[i].testBench + "]",
        			"testBench" : self.variables()[i].testBench,
        			"selected1" : false,
        			"connectedTo" : [],
					"color" : "#1f77b4"
        		}
				variableIds[self.variables()[i].Name + " [" + self.variables()[i].testBench + "]"]= "v"+ i;
        		for(var j = 0; j < self.currentVariables().length; j++){
        			if((self.currentVariables()[j].Name + " [" + self.currentVariables()[j].testBench  + "]")=== varObj.displayName){
        				varObj.selected1 = true;
        				break;
        			}
        		}
				for ( var j = 0; j < self.variables()[i].designs.length; j++ ){
					varObj.connectedTo.push(designIds[self.variables()[i].designs[j]]);
				}
        		variableArray.push(varObj);
        	}

        	for(var i = 0; i < self.configurations().length; i++){
        		var desObj = {
					"id" : "d" + i,
        			"displayName"	: self.configurations()[i].design,
        			"testBench" : self.configurations()[i].testBench,
        			"selected1" : false,
        			"connectedTo" : [],
					"color": self.configurations()[i].color()
        		}
        		for(var j = 0; j < self.currentConfigurations().length; j++){
        			if(self.currentConfigurations()[j].design === desObj.displayName){
        				desObj.selected1 = true;
        				break;
        			}
        		}
        		for(var k = 0; k < self.configurations()[i].variables.length; k++){
        			if(self.configurations()[i].variables[k].type !== 'limit'){
						//console.debug(self.configurations()[i].variables[k].Name+ "[" +self.configurations()[i].variables[k].testBench+ "]");
        				desObj.connectedTo.push(variableIds[self.configurations()[i].variables[k].Name+ " [" +self.configurations()[i].variables[k].testBench+ "]"]);	
        			}        			
        		}
        		designArray.push(desObj);
        	}
          
        	if(self.byVariable()){	
          		out.dataLeft = variableArray;
          		out.dataRight = designArray;            }
            else{
            	out.dataLeft = designArray;
          		out.dataRight = variableArray;
            }
            return out;
		},

		
		/**
        *  pccDistMethods
        *
        *  type: computed array
        *
        *  ?.
        */
		pccDistMethods : function (testBench) {
            console.log("vf-dataprovider: dataproviders.pccDistMethods: enter");
			var out = [];
			var lsc = self.configurations()[self.listOfConfigurations().indexOf(self.selectedConfiguration())];
			for(var i = 0; i < lsc.testBench.length; i++){
				if(lsc.testBench[i].name == testBench && isSet("lsc.testBench[i].PCCResults.byAttr.PCCConfigMetrics", {lsc:lsc, i:i})){
					var methods = Object.keys(lsc.testBench[i].PCCResults.byAttr.PCCConfigMetrics);
					for(var j = 0; j < methods.length; j++){
						var metrics = lsc.testBench[i].PCCResults.byAttr.PCCConfigMetrics[methods[j]];
						for(var k = 0; k < metrics.length; k++){
							if(isSet("metrics[k].Plotting.alpha", {metrics:metrics, k:k})){
								if(!out.inArray(methods[j])){
									out.push(methods[j]);
								}
							}
						}
					}
					return out;
				}
			}
			return [];
		},
		
		/**
        *  pccJointPccAcrossAllTestBenchsMethods -> "pccJointPccMethods"
        *
        *  type: computed array
        *
        *  ?.
        */
		pccJointPccMethods : function (testBench) {
            console.log("vf-dataprovider: dataproviders.pccJointPccMethods: enter");
			var out = [];
			var lsc = self.configurations()[self.listOfConfigurations().indexOf(self.selectedConfiguration())];
			for(var i = 0; i < lsc.testBench.length; i++){
				if(lsc.testBench[i].name == testBench && isSet("lsc.testBench[i].PCCResults.byAttr.JointPCC", {lsc:lsc, i:i})){
					var methods = Object.keys(lsc.testBench[i].PCCResults.byAttr.JointPCC);
					for(var j = 0; j < methods.length; j++){
						out.push(methods[j]);
					}
					return out;
				}
			}
			return [];
		},
		
		/**
        *  pccJointPccMethodsTBvsConfig
        *
        *  type: function
        *
        *  ?.
        */
		pccJointPccMethodsTBvsConfig : function (testBench, config) {
            console.log("vf-dataprovider: dataproviders.pccJointPccMethodsTBvsConfig: enter");
			
			for (var i = 0; i < self.pccResults().length; i++)
			{
				if (self.pccResults()[i].testBench == testBench && self.pccResults()[i].design == config)
				{
					//var lsc = self.configurations()[self.listOfConfigurations().indexOf(config)];
					var selConfig = clone(self.pccResults()[i]);
					
					if (typeof selConfig != "undefined")
					{
						if (isSet("selConfig.PCCResults.byAttr.JointPCC", {selConfig:selConfig, i:i}))
						{
							return Object.keys(selConfig.PCCResults.byAttr.JointPCC);
						}
					}
				}
			}
			return [];
		},
		
		/**
        *  pccJointPccMethodsTBvsConfigWithTime
        *
        *  type: function
        *
        *  ?.
        */
		pccJointPccMethodsTBvsConfigWithTime : function (testBench, config) {
            console.log("vf-dataprovider: dataproviders.pccJointPccMethodsTBvsConfigWithTime: enter");
			
			for (var i = 0; i < self.pccResults().length; i++)
			{
				if (self.pccResults()[i].testBench == testBench && self.pccResults()[i].design == config)
				{
					//var lsc = self.configurations()[self.listOfConfigurations().indexOf(config)];
					var selConfig = clone(self.pccResults()[i]);
					
					if (typeof selConfig != "undefined")
					{
						if (isSet("selConfig.PCCResults.byAttr.JointPCC", {selConfig:selConfig, i:i}))
						{
							var methods = Object.keys(selConfig.PCCResults.byAttr.JointPCC);
							
							for (var j = 0; j < methods.length; j++)
							{
								methods[j] = methods[j] + " " + selConfig.upTime;
							}
							
							return methods;
						}
					}
				}
			}
			return [];
		},
		
		
		/**
        *  pccSensitivityMethodsTBvsConfigWithTime -> "pccSensitivityMethods"
        *
        *  type: function
        *
        *  ?.
        */
		pccSensitivityMethodsTBvsConfigWithTime: function (testBench, config) {			
			console.log("vf-dataprovider: dataproviders.pccSensitivityMethodsTBvsConfigWithTime: enter");
			
			for (var i = 0; i < self.pccResults().length; i++)
			{
				if (self.pccResults()[i].testBench == testBench && self.pccResults()[i].design == config)
				{
					//var lsc = self.configurations()[self.listOfConfigurations().indexOf(config)];
					var selConfig = clone(self.pccResults()[i]);
					
					if (typeof selConfig != "undefined")
					{
						if (isSet("selConfig.PCCResults.byAttr.FirstOrderSensitivity", {selConfig:selConfig, i:i}))
						{
							var methods = Object.keys(selConfig.PCCResults.byAttr.FirstOrderSensitivity);
							
							for (var j = 0; j < methods.length; j++)
							{
								methods[j] = methods[j] + " " + selConfig.upTime;
							}
							
							return methods;
						}
					}
				}
			}
			return [];
		},
		
		
		 /**
        *  pccTable
        *
        *  type: computed array
        *
        *  Builds the table for the pcc tab.  
        */
		pccTable : ko.computed(function () 
		{
			console.log("vf-dataprovider: dataproviders.pccTable: enter");
			
			var out = {
				headers : ['Metric', 'Probability Distribution Function'],
				data : []
			};

			if (self.currentPccSaMethod() != null)
			{
				out.headers.push('Sensitivity');
			}
			
			if (typeof self.currentPccResultObject() != "undefined")
			{
				var currentPccResult = clone(self.currentPccResultObject());
				var currentpccMetrics = currentPccResult.PCCResults.byMethod[self.currentPccUpMethod()].PCCConfigMetrics
				if(currentPccResult.PCCResults.byAttr != "undefined")
				{
					for(var j = 0; j < currentpccMetrics.length; j++)
					{				 
						if (self.currentPccSaMethod() != null)
						{							
							out.data.push([
								currentpccMetrics[j].TestBenchMetricName,
								'<vf-dist id="pccPdf' + j + '" data="vm.dataproviders.pccPDF(\'' + currentPccResult.testBench + '\',\'' + currentPccResult.design + '\',\'' + currentpccMetrics[j].TestBenchMetricName + '\')"></vf-dist>',
								'<vf-pareto id="pccSenitivity' + j + '" minwidth="475" data="vm.dataproviders.pccPareto(\'' + currentPccResult.testBench+'\',\'' + currentpccMetrics[j].TestBenchMetricName + '\')"></vf-pareto>'
							]);
						}
						else
						{	
							out.data.push([
								currentpccMetrics[j].TestBenchMetricName,
								'<vf-dist id="pccPdf'+j+'" data="vm.dataproviders.pccPDF(\'' + currentPccResult.testBench + '\',\'' + currentPccResult.design + '\',\'' + currentpccMetrics[j].TestBenchMetricName + '\')"></vf-dist>'
							]);
						}
					}
				}
			}
			
			return out;
		}, this).extend({
			throttle : 100
		}),
		///////////////////////////// mw///////////////////////////////
		qrTable : ko.computed(function () {
		
			var out = null;
			if(isSet('that.formalverification().length',{that:this})){
				for ( var i = 0; i < this.formalverification().length; i++ )
				{
					if (this.formalverification()[i].ReqName == this.qrSelectedX() &&
						this.formalverification()[i].DesignName == this.qrSelectedY() ){
						out = this.formalverification()[i];
						}
				}
			}
			return out;
		}, this).extend({
			throttle : 100
		}),
		/////////////////////// end of mw///////////////////////////

	
		/**
		*  Outputs the variables for the distribution functions for each of the variables
		*
		**/
		pccPDF : function (testBench, design, metric) 
		{
			console.log("vf-dataprovider: dataproviders.pccPDF: enter");
			var out = {};
			
			for (var i = 0; i < self.pccResults().length; i++)
			{
				if (self.pccResults()[i].testBench === testBench && self.pccResults()[i].design === design)
				{
					if (self.pccResults()[i].PCCResults.byMethod != "undefined" && self.pccResults()[i].PCCResults.byMethod != {})
					{
						//var method = self.pccUPMethodSelected();
						var method = self.currentPccUpMethod();
						
						//var method = self.dataproviders.pccJointPccMethodsTBvsConfig(self.pccResults()[i].testBench, self.pccResults()[i].design)[0];
						//self.pccUPMethodSelected = method;
						//self.pccUPMethodsAvailable(self.dataproviders.pccJointPccMethodsTBvsConfig(self.pccResults()[i].testBench, self.pccResults()[i].design));
						
						if (typeof method != "undefined" || method != null)
						{
							//method = method.split(" ")[0];
							
							for (var j = 0; j < self.pccResults()[i].PCCResults.byMethod[method].PCCConfigMetrics.length; j++)
							{
								if (self.pccResults()[i].PCCResults.byMethod[method].PCCConfigMetrics[j].TestBenchMetricName == metric)
								//if (self.pccResults()[i].PCCResults.byMethod[method].PCCConfigMetrics[j].TestBenchMetricName.match(/\(P\)/,"") == null)
								{
									out = self.pccResults()[i].PCCResults.byMethod[method].PCCConfigMetrics[j];
								}
							}
						}
					}
				}
			}
			
			return out;
		},
				
		/**
        *  pccPareto
        *
        *  type: function
        *
        *  ?.
        */
		pccPareto : function (testBench, metric) {
		    console.log("vf-dataprovider: dataproviders.pccPareto: enter");
			var out = [];
			
			//var lsc = self.configurations()[self.listOfConfigurations().indexOf(self.selectedConfiguration())];
			if (typeof self.currentPccResultObject() != "undefined")
			{
				var currentPccResult = clone(self.currentPccResultObject());
				if(currentPccResult.PCCResults.byAttr != "undefined")
				{
					var method = self.currentPccSaMethod();
					if (typeof method != "undefined" && method != null)
					{			
						if(typeof currentPccResult.PCCResults.byMethod[method] != "undefined")
						{
							var pccResults = currentPccResult.PCCResults.byMethod[method];
							
							var sensitivities = pccResults.FirstOrderSensitivity;
							
							if(typeof currentPccResult.PCCResults.byAttr.PCCConfigMetrics != "undefined")
							{
								var pccConfigMetrics = currentPccResult.PCCResults.byAttr.PCCConfigMetrics[method];
								
								for(var i = 0; i < pccConfigMetrics.length; i++)
								{
									if(pccConfigMetrics[i].TestBenchMetricName == metric)
									{
										for(var j = 0; j < pccResults.InputDistributions.length; j++)
										{
											out.push({
												name : pccResults.InputDistributions[j].TestBenchParameterNames[0],
												value : Math.abs(sensitivities[i][j])
											});					
										}
									}
								}
							}
						}
					}
/* 					if(isSet('pccResults.PCCConfigMetrics.length',{pccResults:pccResults})){
						for(var j = 0; j < pccResults.PCCConfigMetrics.length; j++){
							if(pccResults.PCCConfigMetrics[j].TestBenchMetricName == metric){
								for(var i = 0; i < pccResults.InputDistributions.length; i++){
									out.push({
										name:pccResults.InputDistributions[i].TestBenchParameterNames[0],
										value:Math.abs(sensitivities[j][i])
									});					
								}
							}
						}
					} */
				}
			}
			return out;
		},
		
		
		/**
        *  topsisDesigns
        *
        *  type: function
        *
        *  ?.
        */
		topsisDesigns : ko.computed(function () {
			console.log("vf-dataprovider: dataproviders.topsisDesigns: enter");
			// setup return variable with a static header
			var out = {
				headers : ['Designs', 'Rankings'],
				data : []
			};
			
			// setup topsis input
			var topsisInput = {
				designs:[],
				attributes:[],
				attributeTestBenchs:[],
				optimize:[],
				weights:[],
				matrix:[]
			};
			
			// get design data for topsis starting with all active configurations
			var topsisDesigns = clone(self.activeConfigurations());
			
			// loop through active variables
			for(var i = 0; i < self.topsisVariables().length; i++){
				
				// create list of all active variables within a simple array (easy to search index of)
				topsisInput.attributes.push(self.topsisVariables()[i].Name);
				topsisInput.attributeTestBenchs.push(self.topsisVariables()[i].testBench);
				
				// get active variable's optimization settings
				topsisInput.optimize[i] = self.topsisVariables()[i].optimize(); // 1 for max and -1 for min
				
				// get active variable's weightings
				topsisInput.weights[i] = self.topsisVariables()[i].weighting(); // between 0 and 100
			}
			
			// loop through current configurations
			if(typeof topsisDesigns != 'undefined'){
				for(var i = 0; i < topsisDesigns.length; i++){
					
					// add design name to topsis input
					topsisInput.designs.push(topsisDesigns[i].design);
					
					// loop through each variable within the ith current configuration
					//// use 'a' as a temporary array to be pushed into the input matrix
					var a = [];
					for(var j = 0; j < topsisDesigns[i].variables.length; j++){
						
						// check if jth variable is an active variable
						if(self.isVariableTopsis(topsisDesigns[i].variables[j].testBench, topsisDesigns[i].variables[j].Name)){
							
							// get active variable's position within topsis input
							for(var k = 0; k < topsisInput.attributes.length; k++){
								if(topsisDesigns[i].variables[j].Name == topsisInput.attributes[k] && topsisDesigns[i].variables[j].testBench == topsisInput.attributeTestBenchs[k]){
									var p = k;
									break;
								}
								else{
									var p = -1;
								}
							}
							// get jth variable's value
							a[p] = topsisDesigns[i].variables[j].Value;
						}
					}
					
					// double check input for any undefined values or if the array 'a' is too short
					//// if everything is good then push the temporary array into  the input matrix
					if(a.indexOf(undefined) == -1 && a.length == topsisInput.attributes.length){
					
						// get the current configurations position within the topsis input
						var p = topsisInput.designs.indexOf(topsisDesigns[i].design);
						
						// push temporary array 'a' into position 'p' in the input matrix
						topsisInput.matrix[p] = a;
					}
					
					//// else do not push the temporary array and remove the ith current configuration from the topsis analysis
					else{
						topsisInput.designs.pop();
					}
				}
			}
			
			
			var rankings = topsis(topsisInput);
			self.topsisResults(rankings);
			if(typeof rankings != 'undefined'){
				for(var i = 0; i < rankings.length; i++){
					//out.data.push(['<a onclick="vm.selectedConfiguration(\''+rankings[i].design+'\')">'+rankings[i].design+'</a>', '<div class="progress"><div class="bar" style="width: '+rankings[i].rank * 100+'%;background-color:'+self.getConfigurationByName(rankings[i].design).color()+';"></div></div>']);
					out.data.push(['<a onclick="$(\'#designDetailsModal\').modal(\'show\');vm.selectedConfiguration(\''+rankings[i].design+'\')">'+rankings[i].design+'</a>', '<div class="progress"><div class="bar" style="width: '+rankings[i].rank * 100+'%;background-color:'+self.getConfigurationByName(rankings[i].design).color()+';"></div></div>']);
				}
			}
			
			return out;
		},this).extend({
			throttle : 100
		}),
		
		topsisVariables : function () {
            console.log("vf-dataprovider: dataproviders.topsisVariables: enter");
			var out = {
				headers : ['Variable', 'Test Bench', 'Direction', 'Weight'],
				data : []
			};
			
			var topsisVariables = clone(this.parent.topsisVariables());
			if(typeof topsisVariables != 'undefined'){
				for(var i = 0; i < topsisVariables.length; i++){
					
					if(topsisVariables[i].optimize == 1){
						var min = '';
						var max = 'active';
					}else{
						var min = 'active';
						var max = '';
					}				
					
					out.data.push([
						topsisVariables[i].Name,
						topsisVariables[i].testBench,
						'<center><div class="btn-group" data-toggle="buttons-radio"><button class="btn '+min+'" onclick="vm.dataproviders.topsisChangeOptimize(\''+topsisVariables[i].testBench+'\', \''+topsisVariables[i].Name+'\', -1)">Min</button><button class="btn '+max+'" onclick="vm.dataproviders.topsisChangeOptimize(\''+topsisVariables[i].testBench+'\', \''+topsisVariables[i].Name+'\', 1)">Max</button></div></center>',
						'<input id="range'+i+'" type="range"  min="0" max="100" value="'+topsisVariables[i].weighting+'" onchange="vm.dataproviders.topsisChangeWeights(\''+topsisVariables[i].testBench+'\', \''+topsisVariables[i].Name+'\', \''+"#range"+i+'\')"/>'
					]);
				}
			}
			return out;
		},
		
		topsisChangeWeights : function (testBench, name, id) {
            console.log("vf-dataprovider: dataproviders.topsisChangeWeights: enter");
			for(var i = 0; i < this.parent.topsisVariables().length; i++){
				if(testBench == this.parent.topsisVariables()[i].testBench && name == this.parent.topsisVariables()[i].Name){
					this.parent.topsisVariables()[i].weighting(+$(id).val());
				}
			}
			vf_viz.handlers[0].update();
            
            // Only update stateChanged if not currently set
            if (self.stateChanged() === 0) {
                // Start timer
                self.stateTimer();
                
                // Update status
                self.stateChanged(1);
            }
		},
		
		topsisChangeOptimize : function (testBench, name, direction) {
            console.log("vf-dataprovider: dataproviders.topsisChangeWeights: enter");
			for(var i = 0; i < this.parent.topsisVariables().length; i++){
				if(testBench == this.parent.topsisVariables()[i].testBench && name == this.parent.topsisVariables()[i].Name){
					this.parent.topsisVariables()[i].optimize(direction);
				}
			}
			vf_viz.handlers[0].update();
             
            // Only update stateChanged if not currently set
            console.log("--->"+self.stateChanged());
            if (self.stateChanged() === 0) {
                // Start timer
                self.stateTimer();
                
                // Update status
                self.stateChanged(1);
            }
		}
	};
	
	/**
     *  subscribeAllLeads
     *
     *  type: ?
     *
     *  ?.
     */
	 (function(vm){
		var keys = Object.keys(vm.dataproviders);
		for(var i = 0; i < keys.length; i++){
			if(isSet("vm.dataproviders[keys[i]].subscribe", {keys:keys, i:i, vm:vm})){
				vm.dataproviders[keys[i]].subscribe(function (newValue) {
					// create a delay for event triggers
					try{
						clearTimeout(vm.eventDelayTimer);
					}
					catch(e){

					}

					vm.eventDelayTimer = setTimeout(function(){$(document).trigger('vf-widget-update')}, 250);
				});
			}
		}
	 })(this)
	
      
    /**
     *  variableInArray
     *
     *  type: function
     *
     *  input: array - array of objectw with Name and testBench fields
     *         obj - and object with Name and testBench fields
     *
     *  output: boolean - return true if object is "in" the array
     *
     *  If the Name and testBench fields of the object are
     *  found in the array then return true.
     */
	this.variableInArray = function (array, obj) {
		for (var i = 0; i < array.length; i++) {
			if (array[i].Name == obj.Name && array[i].testBench == obj.testBench) {
				return true;
			}
		}
		return false;
	}
   
    this.activeTimer = -1;
    this.splash = function(mode) {
        console.log("df-dataprovider: splash: enter: "+mode+"  "+this.activeTimer);
        if (mode === 'start') {
            this.activeTimer = setTimeout(function() {
                console.log("df-dataprovider: splash: timer-expired: "+self.activeTimer);
                self.activeTimer = -1;
                if (self.dataLoadComplete) {
                    $('#dataLoadInProgress').modal('hide');
                }
            },3000);
            console.log("df-dataprovider: splash: display modal: "+this.activeTimer);
            $('#dataLoadInProgress').modal('show');
        } else {
            if (this.activeTimer === -1) {
                $('#dataLoadInProgress').modal('hide');
            }
        }    
    }
	
    /**
     *  saveState
     *
     *  type: function
     *
	 *  input: none
     *
     *  output: none
     *
     *  ?.
     */
	this.saveState = function(){
		console.log("df-dataprovider: saveState: enter");
		//var blob = new window.get_blob_builder;
        //var blob;
        var state  = {};
        var item;
        var i;
        
        if (self.stateChanged() !== 0) {
        
            // Project Locations
            state.projectLocation = projectLocation;
            
            // Active variables
            state.activeVariables = [];
            for (i=0; i<self.activeVariables().length;i++) {
                item = self.activeVariables()[i];
                console.log("downloadSettings: activeVariable: "+item.Name);
                state.activeVariables.push({"Name":item.Name,
                                            "Unit":item.Unit,
                                            "isActive":item.isActive(),
                                            "isTopsis":item.isTopsis(),
                                            "optimize":item.optimize(),
                                            "testBench":item.testBench,
                                            "type":item.type,
                                            "weighting":item.weighting()});
            }
        
            // Topsis variables
            state.topsisVariables = [];
            for (i=0; i<self.topsisVariables().length;i++) {
                item = self.topsisVariables()[i];
                console.log("downloadSettings: topsisVariable: "+item.Name);
                state.topsisVariables.push({"Name":item.Name,
                                            "Unit":item.Unit,
                                            "isActive":item.isActive(),
                                            "isTopsis":item.isTopsis(),
                                            "optimize":item.optimize(),
                                            "testBench":item.testBench,
                                            "type":item.type,
                                            "weighting":item.weighting()});
            }
            // Test benches that PET responses belong to
            state.selectedBenchWithPET = self.selectedBenchWithPET();

            // PET Responses
            state.PETResponses = [];
            for (i=0; i<self.listOfPETResponses().length; i++) {
                item = self.listOfPETResponses()[i];
                console.log("item.name="+item.name());
                state.PETResponses.push({"name":item.name(),
                                         "greaterThan":item.greaterThan(),
                                         "display":item.display(),
                                         "value":item.value()
                                        });
                self.PET()[state.selectedBenchWithPET][item.name()].state = {"name":item.name(),
                                         "greaterThan":item.greaterThan(),
                                         "display":item.display(),
                                         "value":item.value()
                                        };
            }
            
            // PET Variables
            state.PETVariables = [];
            for (i=0; i<self.listOfPETVariables().length; i++) {
                item = self.listOfPETVariables()[i];
                console.log("item.name="+item.name());
                state.PETVariables.push({"name":item.name(),
                                         "display":item.display(),
                                         "value":item.value()
                                        });
                for (key in self.PET()[state.selectedBenchWithPET]) {
                    t = self.PET()[state.selectedBenchWithPET][key].inputs;
                    for (j=0; j<t.length; j++) {
                        console.log("key="+key+", "+"j="+j);
                        console.log("label="+t[j].label);
                        console.log("name="+item.name());
                        if (t[j].label === item.name()) {
                            t[j].state = {"name":item.name(),
                                          "display":item.display(),
                                          "value":item.value()
                                         };
                        }
                    }
               }
            }            // Test benches that PET responses belong to
            state.selectedBenchWithPET = self.selectedBenchWithPET();

            // PET Responses
            state.PETResponses = [];
            for (i=0; i<self.listOfPETResponses().length; i++) {
                item = self.listOfPETResponses()[i];
                console.log("item.name="+item.name());
                state.PETResponses.push({"name":item.name(),
                                         "greaterThan":item.greaterThan(),
                                         "display":item.display(),
                                         "value":item.value()
                                        });
                self.PET()[state.selectedBenchWithPET][item.name()].state = {"name":item.name(),
                                         "greaterThan":item.greaterThan(),
                                         "display":item.display(),
                                         "value":item.value()
                                        };
            }
            
            // PET Variables
            state.PETVariables = [];
            for (i=0; i<self.listOfPETVariables().length; i++) {
                item = self.listOfPETVariables()[i];
                console.log("item.name="+item.name());
                state.PETVariables.push({"name":item.name(),
                                         "display":item.display(),
                                         "value":item.value()
                                        });
                for (key in self.PET()[state.selectedBenchWithPET]) {
                    t = self.PET()[state.selectedBenchWithPET][key].inputs;
                    for (j=0; j<t.length; j++) {
                        console.log("key="+key+", "+"j="+j);
                        console.log("label="+t[j].label);
                        console.log("name="+item.name());
                        if (t[j].label === item.name()) {
                            t[j].state = {"name":item.name(),
                                          "display":item.display(),
                                          "value":item.value()
                                         };
                        }
                    }
               }
            }

            ////blob.append(JSON.stringify(state,null,4));
            //blob = new Blob([JSON.stringify(state,null,4)],{type: "application/json;charset="+document.characterSet});
            ////var s=saveAs(blob.getBlob("application/json;charset=" + document.characterSet), "settings.json");
            //var s=saveAs(blob, "settings.json");
            
            // Add session id for server settings
            if (self.serverBased()) {
                state._session_id = document.cookie.match(/_session_id=([^;]+)/)[1];
                console.log("state._session_id: "+state._session_id);
            }
        
            // Convert to JSON
            state = JSON.stringify(state);
            
            // Send to server
            if (self.serverBased()) {
                console.log("<<----------------SENDING TO SERVER--------------------->>");
                //state._session_id = document.cookie.match(/_session_id=([^;]+)/)[1];
                console.log(state);
                $.ajax({
                    url: "/auth/prefs/state/"+self.designHash,
                    type: "post",
                    data: state,
                    success: function() {
                        console.log("settings read: success");
                        self.stateChanged(0);
                    },
                    error: function() {
                        console.log("setting read: failed");
                    }
                });
            } else {
                console.log("<<------------SAVING TO LOCALSTORAGE--------------------->>");
                console.log(state);
                localStorage.setItem(self.designHash,state);
                self.stateChanged(0);
            }
        
            // Clear flag
            //self.stateChanged(0);
        }
	}
    this.downloadSettings = this.saveState;





   
    /**

     *  retrieveState
     *
     *  type: function
     *
     *  input: none
     *
     *  output: none
     *
     *  This function will request the last saved state from the server and restore that state in the current run.
     */
	this.retrieveState = function(){
        console.log("vf-dataprovider: retrieveState: enter");
        

        // Load the last saved state - user the global state variable to load only once
        console.log("state: ");
        console.log(state);
        if (state === null) {
            if (self.serverBased()) {
                console.log("<<----------------RECEIVING FROM SERVER--------------------->>");

                $.ajax({
                    url: "/auth/prefs/state/"+designHash,
                    type: "get",
                    data: state,
                    success: function() {
                        console.log("settings read: success");
                    },
                    error: function() {
                        console.log("setting read: failed");
                    }
                });
            } else {
                console.log("<<------------RETRIEVING FROM LOCALSTORAGE--------------------->>");
                state = localStorage.getItem(self.designHash);
				if (state === null) state = '{"activeVariables":[],"topsisVariables":[]}';
                console.log(state);
                state = JSON.parse(state);
                console.log(state);
            }
        }
    }
   
    this.stateTimer = function() {
        console.log("df-dataprovider: stateTimer: enter");
        if (self.stateChanged() === 0) {
            setTimeout(this.saveState,10000);
        }
    }
    

    this.vmState = function() {
        console.log("vf-dataprovider: vmState: enter");
        dumpVM();
    }
    
    this.mada_ok = function() {
        console.log("vf-dataprovider: mada_ok: enter");
        console.log("\n<<-----------------------------------------------saving state---------------------------------------------->>\n");
        this.stateChanged(1);
        this.saveState();
    
    }
	   
    this.dsa_ok = function() {
        console.log("vf-dataprovider: dsa_ok: enter");
        console.log("\n<<-----------------------------------------------saving state---------------------------------------------->>\n");
        this.stateChanged(1);
        this.saveState();
    
    }
    
    /**
     *  Execute the loadDataFiles function to start loading of data.
     */
    this.splash('start');
    // setTimeout(function() {
        // self.loadDataFiles();
        // },4000);
    this.loadDataFiles();
}
