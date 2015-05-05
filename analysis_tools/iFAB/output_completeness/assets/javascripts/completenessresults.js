$(function() {
  // Get Structural Completeness Results
  $.ajax({url: "completeness_output.json", dataType: "json",
  error: function() {
    var resultsWrapper = $('#results-wrapper');

    if(/chrom(e|ium)/.test(navigator.userAgent.toLowerCase())) {
      resultsWrapper.append('<b>Attention Chrome Users:</b> Chrome has a "security feature" that prevents it from loading the result file.  Please open this webpage in Firefox or Internet Explorer.');
    } else {
      resultsWrapper.append("Failed to load structural completeness results.  Please check your error logs to identify the issue and, if necessary, submit a bug report to VehicleForge.");
    }
  },
  success: function(jsonData) {
    // Define The Page
    var resultsWrapper = $('#results-wrapper');

    // Create The Page Header
    resultsWrapper.append('<h1>Structural Completeness Analysis</h1>');

      var color = "red";

      if (jsonData['completeness_metric'] > 90) {
        color = "green";
      } else if (jsonData['completeness_metric'] > 50) {
        color = "orange";
      }

      // Add Summary / Help Info
      resultsWrapper.append('<h2>About</h2>');
      resultsWrapper.append('<p style="margin: 10px">Structural completeness analyzes the join information in a design (fasteners, welds, adhesives, etc.) to ensure that:</p>');
      resultsWrapper.append('<ol><li>all connections have join information; and</li><li>all joins are valid and complete.</li></ol>');
      resultsWrapper.append('<p style="margin: 10px">Completeness is calculated as <code>[(# of connections) - (# of missing joins) - (# of incomplete joins)] / (# of connections)</code>.  Therefore, a structurally complete design with all joins fully specified will receive a completeness score of 100%. A design lacking all join information will score 0%. The results below identify each connection with missing or invalid join information and provide a short description of the problem.  These results also highlight any unattached components.  A component is unattached if it is not connected to any other component, which may indiciate an issue in the design.</p>');

      // Add Summary Data
      resultsWrapper.append('<h2>Summary</h2>');
      var summaryTable = $('<table />').addClass('table table-striped table-hover');
      summaryTable.append('<tr><td width="20%">Completeness Metric:</td><td><font color="' + color + '">' + jsonData['completeness_metric'].toFixed(2) + ' %</font></td></tr>');
      summaryTable.append('<tr><td width="20%">Number of Components:</td><td>' + jsonData['number_of_components'] + '</td></tr>');
      summaryTable.append('<tr><td width="20%">Number of Unattached Components:</td><td>' + jsonData['number_of_free_components'] + '</td></tr>');
      summaryTable.append('<tr><td width="20%">Number of Connections:</td><td>' + jsonData['number_of_connections'] + '</td></tr>');
      summaryTable.append('<tr><td width="20%">Connections Missing Join Data:</td><td>' + jsonData['missing_join_data'] + '</td></tr>');
      summaryTable.append('<tr><td width="20%">Connections with Incomplete Join Data:</td><td>' + jsonData['incomplete_join_data'] + '</td></tr>');
      resultsWrapper.append(summaryTable);

      resultsWrapper.append('<h2>Identified Issues</h2>');
      var history = {};
      var count = 0
      var detailsTable = $('<table id="details" />').addClass('table table-striped table-hover');
      detailsTable.append('<thead><tr><th></th><th>Part A</th><th>Part B</th><th>Issue</th></tr></thead>');
      $.each(jsonData.details, function(index, componentA) {
        $.each(componentA.connections, function(index, componentB) {
          if (componentB.warnings.length > 0) {
            var warningsList = "";

            $.each(componentB.warnings, function(index, warning) {
              if (warningsList.length > 0) {
                warningsList += ", ";
              }

              warningsList += warning;
            });

            // avoid adding for duplicates
            var key1 = componentA.id + " - " + componentB.id + " - " + warningsList;
            var key2 = componentB.id + " - " + componentA.id + " - " + warningsList;

            if (!(key1 in history) && !(key2 in history)) {
              history[key1] = true;
              detailsTable.append('<tr><td style="text-align: center" width="20px"><img src="assets/images/Failed.png" /></td><td width="15%">' + componentA.name + '</td><td width="15%">' + componentB.name + '</td><td>' + warningsList + '</tr>');
              count++;
            }
          }
        });
      });

      if (count == 0) {
        detailsTable = "No issues identified."
      }

      resultsWrapper.append(detailsTable);

      if (count > 0) {
        sorttable.makeSortable(document.getElementById('details'));
      }

      $('img.tip').qtip({
        style: {classes: 'qtip-bootstrap qtip-shadow'}
      });
  }});
});

function addCommas(nStr)
{
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