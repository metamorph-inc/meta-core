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
		
    self.designHash = CryptoJS.SHA256(projectLocation);

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
			self.theme(data.theme);
            self.loadPending();
        },
        error: function(jqXHR, textStatus, errorThrown){
            console.error("vf-dataproviders: loadDataFiles: error(manifest.json): ",errorThrown);
            if(jqXHR.statusText == "OK"){jqXHR.statusText = textStatus;}
            self.loadPending();
        }
    }));
	
	// Retrieve the cost / lead-time data
	if(this.vfMode()){	
		var clPath = '/fang/'+projectLocation.split('/')[2]+'/home/design_submission/manufacturability_results';
		console.log("vf-dataproviders: loadDataFiles: ajax: " + clPath);
		n.push(clPath);
			h.push($.ajax({
			url : clPath,
			dataType : 'json',
			context : this,
			success : function (data) {
				console.log("vf-dataproviders: loadDataFiles: success: " + clPath);
				self.costLeadTimeArray(data.results);
				self.loadPending();
			},
			error: function(jqXHR, textStatus, errorThrown){
				console.error("vf-dataproviders: loadDataFiles: error(manifest.json): ",errorThrown);
				if(jqXHR.statusText == "OK"){jqXHR.statusText = textStatus;}
				self.loadPending();
			}
		}));
	}
	
	//processing functions
	this.processResultsSummaries = function(summaries){
	
	}
	
    // Retrieve the meta-data files
    console.log("vf-dataproviders: loadDataFiles: ajax(projectLocation): "+projectLocation);
    n.push(projectLocation);
    h.push($.ajax({
        url : projectLocation,
        dataType : 'json',
        self : self,
        success : function (avmproject, status, xhr) {
            console.log("vf-dataproviders: loadDataFiles: success(projectLocation): "+projectLocation);
            //self.numberOfFilesToLoad = 4 + avmproject.Project.NumberOfReferencedFiles; (NOT USED ANYMORE)
            console.log("\nvf-dataproviders: loadDataFiles: success(projectLocation): Results(level 1) - imax="+avmproject.Project.Results.UrlHints.length);
            for (var i = 0; i < avmproject.Project.Results.UrlHints.length; i++) {
				if(typeof avmproject.Project.Results.UrlHints[i] === string){
					var iUrl = projectLocation.split('/').slice(0, -1).join('/') + '/' + avmproject.Project.Results.UrlHints[i];
					iUrl = iUrl.split('\\').join('/');
					iUrl = encodeURI(iUrl);
					if(visualizerMode == "true"){
						iUrl = window.location.pathname.split('/').slice(0,-1).join('/') + iUrl;
					}
						
					// Retrieve
					console.log("vf-dataproviders: loadDataFiles: ajax(iUrl): "+iUrl+"?format=raw");
					n.push(iUrl);
					h.push($.ajax({
						url : iUrl + '?format=raw',
						dataType : 'json',
						self : self,
						success : function (metaResult, status, xhr) {
							console.log("vf-dataproviders: loadDataFiles: success(iUrl): "+iUrl+"?format=raw");
							console.log("vf-dataproviders: loadDataFiles: success(iUrl): Results(level 2) - jmax="+
								metaResult.Results.length);
										
							// 
							for (var j = 0; j < metaResult.Results.length; j++) {
								var jUrl = iUrl.split('/').slice(0, -1).join('/') + '/' + this.url.split(this.url.split("\\")[this.url.split("\\").length - 1])[0] + metaResult.Results[j].Summary;
								jUrl = jUrl.split('\\').join('/');
								jUrl = encodeURI(jUrl);
									
								// Create the SUCCESS function template
								var successFunctionTemplate = function(url, self){
									// Return the actual function
									return function(report, textStatus, jqXHR){
										console.log("vf-dataproviders: loadDataFiles: success(jUrl): "+jUrl+"?format=raw");
										var config = {
											design : report.DesignName,
											designId: 'CID' + report.DesignID.split("-").join("").split("{").join("").split("}").join(""),
											time : report.Time,
											isSelected : false,
											isActive : true,
											testBench : report.TestBench,
											variables : [],
											responses : report.responses,
											PCCResults: report.PCCResults,
											requirementsResults : 'white'
										}
										
										// Add limitInfo
										if(isSet('report.LimitChecks', {report:report})){	
										  config.limitInfo = report.LimitChecks;
										}
											
										// Add the Parameters to the variable list for the configuration
										for (var i = 0; i < report.Parameters.length; i++) {
											if(report.Parameters[i].Value != ''){
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
											if(report.Metrics[i].Value != ''){
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
												config.variables.push(report.Metrics[i]);
											} else{
												jqXHR.statusText = 'Contains blank values';
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
											  console.log(limitInfo);
											  config.variables.push(limitInfo);
											}
										  }
										}
										
										// Add variable file locations
										for (var i = 0; i < config.variables.length; i++) {
											config.variables[i].location = url.split('/').slice(0,-1).join('/');
										}
											
										// Add the configuration entry to the list
										self.allConfigurations.push(config);
										// self.allConfigurations()[self.allConfigurations().length - 1].isSelected.subscribe(function (newValue) {
										// $(window).trigger('vf-selectionChange');
										// });
									};
								}
									
								// Setup the SUCCESS function for the following
								successFunction = successFunctionTemplate(jUrl, self);
									
								console.log("vf-dataproviders: loadDataFiles: ajax(jUrl): "+jUrl+"?format=raw");
								n.push(jUrl);
								h.push($.ajax({
									url : jUrl + '?format=raw',
									dataType : 'json',
									self : self,
									success : successFunction,
									complete: function(){
										console.log("vf-dataproviders: loadDataFiles: complete(jUrl): "+jUrl+"?format=raw");
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
					for (var j = 0; j < avmproject.Project.Results.UrlHints[i].Results.length; j++) {
						self.processResultsSummaries();
					}
				}
				console.log("\nvf-dataproviders: loadDataFiles: success(projectLocation): Requirements - kmax="+
					avmproject.Project.Requirements.UrlHints.length);
				if (self.vfMode()) {
					// Read requirements from the server (this depends on the server placing the format parameter in the parameters)
					console.log("REQUIREMENTS from server");
					// if (avmproject.hasOwnProperty('vfLink')) {
						// kUrl = avmproject.Project.Requirements.vfLink;
					// } else {
						kUrl = "/rest/fang/standings/get_requirements?return_all=true&format=raw";
					//}
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
					for (var k = 0; k < avmproject.Project.Requirements.UrlHints.length; k++) {
						var kUrl = projectLocation.split('/').slice(0, -1).join('/') + '/' + avmproject.Project.Requirements.UrlHints[k];
						kUrl = kUrl.split('\\').join('/');
						kUrl = encodeURI(kUrl);
						if(visualizerMode == "true"){
							kUrl = window.location.pathname.split('/').slice(0,-1).join('/') + kUrl;
						}
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
				}
				}
            console.log("\nvf-dataproviders: loadDataFiles: success(projectLocation): Configurations - lmax="+
                avmproject.Project.Configurations.length);
            for (var l = 0; l < avmproject.Project.Configurations.length; l++) {
                var lUrl = projectLocation.split('/').slice(0, -1).join('/') + '/' + avmproject.Project.Configurations[l];
                lUrl = lUrl.split('\\').join('/');
                lUrl = encodeURI(lUrl);
                if(visualizerMode == "true"){
                    lUrl = window.location.pathname.split('/').slice(0,-1).join('/') + lUrl;
                }
                console.log("vf-dataproviders: loadDataFiles: ajax(lUrl): "+lUrl+"?format=raw");
                n.push(lUrl);
                h.push($.ajax({
                    url : lUrl + '?format=raw',
                    dataType : 'json',
                    self : self,
                    success : function (com, status, xhr) {
                        console.log("vf-dataproviders: loadDataFiles: success(lUrl): "+lUrl+"?format=raw");
                        var out = {
                            design: com.Name,
                            cid: com.Name,
                            componets: []
                        };
							
                        var searchInObject = function (x) {
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
            
            console.log("\nvf-dataproviders: loadDataFiles: success(projectLocation): DesignSpaceModels - mmax="+
                avmproject.Project.DesignSpaceModels.length);
            for (var m = 0; m < avmproject.Project.DesignSpaceModels.length; m++) {
                var mUrl = projectLocation.split('/').slice(0, -1).join('/') + '/' + avmproject.Project.DesignSpaceModels[m];
                mUrl = mUrl.split('\\').join('/');
                mUrl = encodeURI(mUrl);
                if(visualizerMode == "true"){
                    mUrl = window.location.pathname.split('/').slice(0,-1).join('/') + mUrl;
                }
                console.log("vf-dataproviders: loadDataFiles: ajax(mUrl): "+mUrl+"?format=raw");
                n.push(mUrl);
                h.push($.ajax({
                    url : mUrl + '?format=raw',
                    dataType : 'json',
                    self : self,
                    success : function (com, status, xhr) {
                        console.log("vf-dataproviders: loadDataFiles: success(mUrl): "+mUrl+"?format=raw");
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