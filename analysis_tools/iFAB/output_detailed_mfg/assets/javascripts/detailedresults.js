$(function() {
  // Get Detailed Manufacturability Results
  $.ajax({url: "detailed.manufacturabilityResults.json", dataType: "json",
  error: function() {
    var resultsWrapper = $('#results-wrapper');

    if(/chrom(e|ium)/.test(navigator.userAgent.toLowerCase())) {
      resultsWrapper.append('<b>Attention Chrome Users:</b> Chrome has a "security feature" that prevents it from loading the result file.  Please open this webpage in Firefox or Internet Explorer.');
    } else {
      resultsWrapper.append("Failed to load detailed manufacturability results.  Please check your error logs to identify the issue and, if necessary, submit a bug report to VehicleForge.");
    }
  },
  success: function(jsonData) {

    // Define The Page
    var resultsWrapper = $('#results-wrapper');

    // Create The Page Header
    resultsWrapper.append('<h1>Detailed Manufacturability Analysis</h1>');

    if (jsonData[0].manufacturable) {
      // Get The Manufactured & Purchased Aggerate Data
      var purchasedPartCost    = 0;
      var purchasedPartTime    = 0;
      var manufacturedPartCost = 0;
      var manufacturedPartTime = 0;
      var purchasedCount       = 0;
      var manufacturedCount    = 0;
      var maxMakespan = 0;

      $.each(jsonData[0].bestScore.manufacturingDetails, function(index, part) {
        if (part.type == "purchased") {
          purchasedPartCost += part['cost($)'];
          purchasedPartTime = Math.max(purchasedPartTime, part['time(min)']);
          purchasedCount += 1;
        } 
        if (part.type == "manufactured") {
          manufacturedPartCost += part['cost($)'];
          manufacturedPartTime += part['time(min)'];
          manufacturedCount += 1;
        }

        maxMakespan = Math.max(maxMakespan, makespan(part));
      });

      // Add Summary Data
      var summaryTable = $('<table>');
      summaryTable.addClass('table table-striped table-hover');
      summaryTable.append('<tr><td style="width: 250px;">Message:</td><td>' + jsonData[0].message + '</td></tr>');
      summaryTable.append('<tr><td>Number of Purchased Parts:</td><td>' + purchasedCount + '</td></tr>');
      summaryTable.append('<tr><td>Number of Manufactured Parts:</td><td>' + manufacturedCount + '</td></tr>');
      summaryTable.append('<tr><td><b>Total Cost:</b></td><td><b>$' + addCommas(jsonData[0].paretoFront[0]['cost($)'].toFixed(2)) + '</b> <img id="totalCostTip" src="assets/images/icon_info.gif" /></td></tr>');
      summaryTable.append('<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Purchased Part Cost:</td><td>$' + addCommas(purchasedPartCost.toFixed(2)) + '</td></tr>');
      summaryTable.append('<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Manufactured Part Cost:</td><td>$' + addCommas(manufacturedPartCost.toFixed(2)) + '</td></tr>');
      summaryTable.append('<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Assembly Cost:</td><td>' + (jsonData[0].bestScore.assemblyDetails ? '$' + addCommas(jsonData[0].bestScore.assemblyDetails['cost($)'].toFixed(2)) : 'N/A') + '</td></tr>');
      summaryTable.append('<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Matl Handling and Overhead:</td><td>$' + addCommas(Math.max(0.0, jsonData[0].paretoFront[0]['cost($)'] - purchasedPartCost - manufacturedPartCost - (jsonData[0].bestScore.assemblyDetails ? jsonData[0].bestScore.assemblyDetails['cost($)'] : 0)).toFixed(2)) + '</td></tr>');
      summaryTable.append('<tr><td><b>Total Time:</b></td><td><b>' + toTime(jsonData[0].paretoFront[0]['time(min)']) + '</b> <img id="totalTimeTip" src="assets/images/icon_info.gif" /></td></tr>');
      summaryTable.append('<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Purchased Part Lead Time:</td><td>' + toTime(purchasedPartTime) + '</td></tr>');
      summaryTable.append('<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Manufactured Part Time:</td><td>' + toTime(manufacturedPartTime) + '</td></tr>');
      summaryTable.append('<tr><td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Assembly Time:</td><td>' + (jsonData[0].bestScore.assemblyDetails ? toTime(jsonData[0].bestScore.assemblyDetails['time(min)']) : 'N/A') + '</td></tr>');

      resultsWrapper.append('<h2>Summary</h2></br>');
      resultsWrapper.append('<table width="100%"><tr><td width="100%">' + summaryTable[0].outerHTML + '</td><td width="300px"><div id="costChart" style="height:400px;width:300px;"></div></td></tr></table>');

      // Add the Parts
      if (purchasedCount + manufacturedCount != 0) {
        resultsWrapper.append('<h2>Part Information</h2>');
        var partTable = $('<table id="parts-table" />').addClass('table table-striped table-hover');
        partTable.append('<thead><tr><th>Part</th><th>Type</th><th style="text-align: center;">Quantity</th><th class="sorttable_nosort"></th><th style="text-align: center;">Cost</th><th class="sorttable_nosort"></th><th class="sorttable_nosort"></th><th colspan=3 style="text-align: center;">Lead Time <img id="leadTimeTip" class="right-tip" src="assets/images/icon_info.gif" /></th><th class="sorttable_nosort" width="10%"></th></tr></thead>');
        resultsWrapper.append(partTable);
      }

      // Loop Through The Data & Add To Parts Tables
      $.each(jsonData[0].bestScore.manufacturingDetails, function(index, part) {
        // Set Progress %
        var costProgress = (purchasedPartCost + manufacturedPartCost) == 0 ? 0 : (part['cost($)']   / (purchasedPartCost + manufacturedPartCost) * 100);
        var timeProgress = maxMakespan == 0 ? 0 : (makespan(part) / maxMakespan * 100);
        
        // Create Row Data
        var row = $(
          '<tr>' +
            '<td>' + part['name'] + (part["detailedFeedback"] ? ' <a title="View manufacturing feedback PDF" href="feedback/' + part["detailedFeedback"]["fileID"] + '.pdf"><img src="assets/images/page_white_acrobat.png" /></a>' : '') + '</td>' +
            '<td>' + capitalize(part['type']) + '</td>' +
            '<td style="text-align: center;">' + part['quantity'] + '</td>' +
            '<td style="width: 80px; text-align: right;">$' + addCommas(part['cost($)'].toFixed(2)) + '</td>' +
            '<td sorttable_customkey="' + part['cost($)'] + '" style="width: 50px; text-align: right;">' + costProgress.toFixed(1) + '%</td>' +
            '<td style="width: 100px; text-align: left;">' +
              '<div class="progress">' +
                '<div class="progress-bar"  role="progressbar" aria-valuenow="' + costProgress.toFixed(0) + '" aria-valuemin="0" aria-valuemax="100" style="width: ' + costProgress.toFixed(0) + '%"></div>' +
              '</div>' + 
            '</td>' +
            '<td class="spacer-column"></td>' +
            '<td sorttable_customkey="' + makespan(part) + '" style="width: 80px; text-align: right;">' + toTime(makespan(part)) + '</td>' +
            '<td style="width: 50px; text-align: right;">' + timeProgress.toFixed(1) + '%</td>' +
            '<td style="width: 100px; text-align: left;">' +
              '<div class="progress">' +
                '<div class="progress-bar"  role="progressbar" aria-valuenow="' + timeProgress.toFixed(0) + '" aria-valuemin="0" aria-valuemax="100" style="width: ' + timeProgress.toFixed(0) + '%"></div>' +
              '</div>' + 
            '</td>' +
            '<td class="spacer-column"></td>' +
          '</tr>'
        );

        // Add Row To Correct Table
        partTable.append(row);
      });

      // Allow Tables To Sort
      if (purchasedCount + manufacturedCount != 0) {
        sorttable.makeSortable(document.getElementById('parts-table'));
      }

      // Add The Gantt Chart
      resultsWrapper.append('<h2 style="margin-bottom: 0pt">Schedule</h2>');
      resultsWrapper.append('<div style="margin-top: 0pt" class="gantt"></div>');

      var sourceData = [];

      $.each(jsonData[0].bestScore.manufacturingDetails, function(index, part) {
        sourceData.push({
          name: part["name"],
          sortKey: new Date(part["startTime"]).getTime(),
          values: [{
            from: "/Date(" + new Date(part["startTime"]).getTime() + ")/",
            to: "/Date(" + new Date(part["completeTime"]).getTime() + ")/",
            label: part["name"],
          }]
        });
      });

      sourceData.sort(compare);

      if (jsonData[0].bestScore.assemblyDetails) {
        sourceData.push({
          name: "Assembly",
          sortKey: new Date(jsonData[0].bestScore.assemblyDetails.startTime).getTime(),
          values: [{
            from: "/Date(" + new Date(jsonData[0].bestScore.assemblyDetails.startTime).getTime() + ")/",
            to: "/Date(" + new Date(jsonData[0].bestScore.assemblyDetails.completeTime).getTime() + ")/",
            label: "Assembly"
          }]
        });
      }
      
      $(".gantt").gantt({
        source: sourceData,
        navigate: "scroll",
        maxScale: "months",
        minScale: "days",
        scrollToToday: false,
        itemsPerPage: purchasedCount + manufacturedCount + 1
      });

      $('img#totalCostTip').qtip({
        content: '<table><tr><td style="vertical-align:top;white-space:nowrap"><b>Total Cost:</b></td><td>The total cost to produce one unit of your design.</td></tr><tr><td style="vertical-align:top;white-space:nowrap"><b>Purchased Part Cost:</b></td><td>The total cost for purchased parts.</td></tr><tr><td style="vertical-align:top;white-space:nowrap"><b>Manufactured Part Cost:</b></td><td>The total cost to manufacture custom parts.</td></tr><tr><td style="vertical-align:top;white-space:nowrap"><b>Assembly Cost:</b></td><td>The costs for positioning, aligning, and fastening the parts into the final assembly.</td></tr><tr><td style="vertical-align:top;white-space:nowrap"><b>Matl Handling and Overhead:</b></td><td>All other expenses incurred.  This includes logisitics, material handling, and all overhead for operating the foundry.</td></tr></table>',
        style: {classes: 'qtip-bootstrap qtip-shadow'}
      });

      $('img#totalTimeTip').qtip({
        content: '<table><tr><td style="vertical-align:top;white-space:nowrap"><b>Total Time:</b></td><td style="vertical-align:top;">The total makespan that accounts for weekends, holidays, material handling, and resource constraints.</td></tr><tr><td style="vertical-align:top;white-space:nowrap"><b>Purchased Part Lead Time:</b></td><td style="vertical-align:top;">The maximum lead time for purchased parts.</td></tr><tr><td style="vertical-align:top;white-space:nowrap"><b>Manufactured Part Time:</b></td><td style="vertical-align:top;">The total time spent manufacturing parts (machining, casting, etc.).  Shipment of raw materials adds approximately two weeks to the lead time for manufactured parts.</td></tr><tr><td style="vertical-align:top;white-space:nowrap"><b>Assembly Time:</b></td><td style="vertical-align:top;">The total time spent assembling and fastening the parts.</td></tr></table>',
        style: {classes: 'qtip-bootstrap qtip-shadow'}
      });

      $('img#leadTimeTip').qtip({
        content: 'Lead time percentages are scaled based on the longest lead time.  Changing the longest lead time parts may result in significant reductions in overall manufacturing time.',
        style: {classes: 'qtip-bootstrap qtip-shadow'}, tip: { offset: 100 }
      });

      // Create charts
      var costData = [
        ['Purchased Parts', purchasedPartCost],
        ['Manufactured Parts', manufacturedPartCost],
        ['Assembly', jsonData[0].bestScore.assemblyDetails ? jsonData[0].bestScore.assemblyDetails['cost($)'] : 0.0],
        ['Matl Handling and Overhead', Math.max(0.0, jsonData[0].paretoFront[0]['cost($)'] - purchasedPartCost - manufacturedPartCost - (jsonData[0].bestScore.assemblyDetails ? jsonData[0].bestScore.assemblyDetails['cost($)'] : 0))]];

      var plot1 = jQuery.jqplot('costChart', [costData], {
        seriesDefaults: {
          renderer: jQuery.jqplot.PieRenderer,
          rendererOptions: {
            showDataLabels: true
          }
        },
        legend: { show:true, location: 's', border: 'none' },
        title: 'Cost Breakdown'
      });
    } else {
      var displayError = true;

      if (jsonData[0].manufacturabilityAssessment) {
        $.each(jsonData[0].manufacturabilityAssessment, function(index, part) {
          if (!part['manufacturable']) {
            displayError = false;
          }
        });
      }

      resultsWrapper.append('<h2>Summary</h2></br>');
      var summaryTable = $('<table />').addClass('table table-striped table-hover');
      summaryTable.append('<tr><td style="width: 250px;">Message:</td><td><font color="red">Design is not manufacturable!</font></td></tr>');

      if (displayError) {
        summaryTable.append('<tr><td style="width: 250px;">Error:</td><td>' + jsonData[0].message + '</td></tr>');
      }

      var partCost         = 0;
      var partTime         = 0;
      var partSuccessCount = 0;
      var partFailureCount = 0;

      if (jsonData[0].manufacturabilityAssessment) {
        $.each(jsonData[0].manufacturabilityAssessment, function(index, part) {
          if (part.manufacturable) {
            if (part.quantity) {
              partCost += part['quantity'] * part['cost($)'];
            } else {
              partCost += part['cost($)'];
            }
            partTime = Math.max(partTime, part['time(min)']);
            partSuccessCount += 1;
          } else {
            partFailureCount += 1;
          }
        });

        // Add Summary Data
        summaryTable.append('<tr><td>Manufacturable Parts:</td><td>' + partSuccessCount + '</td></tr>');
        summaryTable.append('<tr><td>Non-manufacturable Parts:</td><td>' + partFailureCount + '</td></tr>');
        summaryTable.append('<tr><td><b>Partial Cost:</b></td><td><b>$' + addCommas(partCost.toFixed(2)) + '</b></td></tr>');
        summaryTable.append('<tr><td><b>Partial Lead Time:</b></td><td><b>' + toTime(partTime) + '</b></td></tr>');

        if (jsonData[0].assemblyAssessment) {
          var row = $(
            '<tr>' +
              '<td>Assembly:</td>' +
              '<td>' + (jsonData[0].assemblyAssessment.assemblable ? 'Success' : 'The design could not be assembled.') + '</td>' +
            '</tr>'
          );
          summaryTable.append(row);
        }
      }

      resultsWrapper.append(summaryTable);

      if (jsonData[0].manufacturabilityAssessment) {
        // Add the Parts
        if (partSuccessCount > 0) {
          resultsWrapper.append('<h2>Part Information</h2>');
          var partTable = $('<table id="parts-table" />').addClass('table table-striped table-hover');
          partTable.append('<thead><tr><th class="sorttable_nosort"></th><th>Part</th><th>Type</th><th style="text-align: center;">Quantity</th><th class="sorttable_nosort"></th><th style="text-align: center;">Cost</th><th class="sorttable_nosort"></th><th class="sorttable_nosort"></th><th colspan=3 style="text-align: center;">Lead Time <img id="leadTimeTip" class="right-tip" src="assets/images/icon_info.gif" /></th><th class="sorttable_nosort" width="10%"></th></tr></thead>');
          resultsWrapper.append(partTable);
        }

        if (partFailureCount > 0) {
          resultsWrapper.append('<h2>Manufacturability Feedback</h2>');
          var issuesTable = $('<table id="issues-table" />').addClass('table table-striped table-hover');
          issuesTable.append('<thead><tr><th class="sorttable_nosort"></th><th>Part</th><th class="sorttable_nosort">Message</th></tr></thead>');
          resultsWrapper.append(issuesTable);
        }

        // Loop Through The Data & Add To Parts Tables
        $.each(jsonData[0].manufacturabilityAssessment, function(index, part) {
          if (part['manufacturable']) {
            // Set Progress %
            var costProgress = partCost == 0 ? 0 : (part['cost($)']   / partCost * 100);
            var timeProgress = partTime == 0 ? 0 : (part['time(min)'] / partTime * 100);
            
            // Create Row Data
            var row = $(
              '<tr>' +
                '<td style="text-align: center" width="20px"><img src="assets/images/Success.png" /></td>' +
                '<td>' + part['name'] + (part["detailedFeedback"] ? ' <a title="View manufacturing feedback PDF" href="feedback/' + part["detailedFeedback"]["fileID"] + '.pdf"><img src="assets/images/page_white_acrobat.png" /></a>' : '') + '</td>' +
                '<td>' + capitalize(part['type'] ? part['type'] : 'Unknown') + '</td>' +
                '<td style="text-align: center;">' + (part.quantity ? part['quantity'] : 1) + '</td>' +
                '<td style="width: 80px; text-align: right;">$' + addCommas(part['cost($)'].toFixed(2)) + '</td>' +
                '<td sorttable_customkey="' + part['cost($)'] + '" style="width: 50px; text-align: right;">' + costProgress.toFixed(1) + '%</td>' +
                '<td style="width: 100px; text-align: left;">' +
                  '<div class="progress">' +
                    '<div class="progress-bar"  role="progressbar" aria-valuenow="' + costProgress.toFixed(0) + '" aria-valuemin="0" aria-valuemax="100" style="width: ' + costProgress.toFixed(0) + '%"></div>' +
                  '</div>' + 
                '</td>' +
                '<td class="spacer-column"></td>' +
                '<td sorttable_customkey="' + part['time(min)'] + '" style="width: 80px; text-align: right;">' + toTime(part['time(min)']) + '</td>' +
                '<td style="width: 50px; text-align: right;">' + timeProgress.toFixed(1) + '%</td>' +
                '<td style="width: 100px; text-align: left;">' +
                  '<div class="progress">' +
                    '<div class="progress-bar"  role="progressbar" aria-valuenow="' + timeProgress.toFixed(0) + '" aria-valuemin="0" aria-valuemax="100" style="width: ' + timeProgress.toFixed(0) + '%"></div>' +
                  '</div>' + 
                '</td>' +
                '<td class="spacer-column"></td>' +
              '</tr>'
            );

            // Add Row To Correct Table
            partTable.append(row);
          } else {
            var row = $(
              '<tr>' +
                '<td style="text-align: center" width="20px"><img src="assets/images/Failed.png" /></td>' +
                '<td>' + part['name'] + (part['detailedFeedback'] ? ' <a title="View manufacturing feedback PDF" href="feedback/' + part["detailedFeedback"]["fileID"] + '.pdf"><img src="assets/images/page_white_acrobat.png" /></a>' : '') + '</td>' +
                '<td>' + part['simpleFeedback'] + '</td>' +
              '</tr>'
            );
            issuesTable.append(row);
          }
        });

        // Allow Tables To Sort
        if (partSuccessCount != 0) {
          sorttable.makeSortable(document.getElementById('parts-table'));
        }

        resultsWrapper.append('<h2>Manufacturability Feedback</h2>');

        $('img#leadTimeTip').qtip({
          content: 'Lead time percentages are scaled based on the longest lead time.  Changing the longest lead time parts may result in significant reductions in overall manufacturing time.',
          style: {classes: 'qtip-bootstrap qtip-shadow'}, tip: { offset: 100 }
        });
      }
    }
  }});
});

function capitalize(s) {
  return s[0].toUpperCase() + s.slice(1);
}

function makespan(part) {
  var start = new Date(part["startTime"]).getTime();
  var end = new Date(part["completeTime"]).getTime();
  return (end - start) / 60000;
}

function toTime(mins) {
  if (mins > 24*60) {
    return (mins / (24.0*60.0)).toFixed(1) + '&nbsp;days';
  } else if (mins > 60) {
    return (mins / 60.0).toFixed(1) + '&nbsp;hrs';
  } else {
    return mins.toFixed(1) + '&nbsp;mins';
  }
}

function addCommas(nStr) {
  nStr += '';
  x = nStr.split('.');
  x1 = x[0];
  x2 = x.length > 1 ? '.' + x[1] : '';
  var rgx = /(\d+)(\d{3})/;
  while (rgx.test(x1)) {
    x1 = x1.replace(rgx, '$1' + ',' + '$2');
  }
  return x1 + x2;
}

function compare(a, b) {
  if (a.sortKey < b.sortKey) {
    return -1;
  } else if (a.sortKey > b.sortKey) {
    return 1;
  } else {
    return 0;
  }
}