$(function() {
  // Get Detailed Manufacturability Results
  $.getJSON("detailed.manufacturabilityResults.json", function(jsonData) {
      // Define The Page
      var resultsWrapper = $('#results-wrapper');

      // Create The Page Header
      resultsWrapper.append('<h1>Detailed Manufacturability Analysis</h1>');

      // Get The Manufactured & Purchased Aggerate Data
      var purchasedPartCost    = 0;
      var purchasedPartTime    = 0;
      var manufacturedPartCost = 0;
      var manufacturedPartTime = 0;
      var purchasedCount       = 0;
      var manufacturedCount    = 0;

      $.each(jsonData[0].bestScore.manufacturingDetails, function(index, part) {
        if (part.type == "purchased") {
          purchasedPartCost += part['cost($)'];
          purchasedPartTime += part['time(min)'];
          purchasedCount += 1;
        } 
        if (part.type == "manufactured") {
          manufacturedPartCost += part['cost($)'];
          manufacturedPartTime += part['time(min)'];
          manufacturedCount += 1;
        }
      });

      // Add Summary Data
      resultsWrapper.append('<h2>Summary</h2></br>');
      var summaryTable = $('<table />').addClass('table table-striped table-hover');
      summaryTable.append('<tr><td style="width: 250px;">Message:</td><td>' + jsonData[0].message + '</td></tr>');
      summaryTable.append('<tr><td>Pareto Front Score:</td><td>' + addCommas(jsonData[0].paretoFront[0].score.toFixed(2)) + '</td></tr>');
      summaryTable.append('<tr><td>Purchased Parts:</td><td>' + purchasedCount + '</td></tr>');
      summaryTable.append('<tr><td>Manufactured Parts:</td><td>' + manufacturedCount + '</td></tr>');
      summaryTable.append('<tr><td>Pareto Front Time (min):</td><td>' + jsonData[0].paretoFront[0]['time(min)'].toFixed(1) + '</td></tr>');
      summaryTable.append('<tr><td>Purchased Part Time (min):</td><td>' + purchasedPartTime.toFixed(1) + '</td></tr>');
      summaryTable.append('<tr><td>Manufactured Part Time (min):</td><td>' + manufacturedPartTime.toFixed(1) + '</td></tr>');
      summaryTable.append('<tr><td>Assembly Time (min):</td><td>' + jsonData[0].bestScore.assemblyDetails['time(min)'].toFixed(1) + '</td></tr>');
      summaryTable.append('<tr><td>Pareto Front Cost:</td><td>$' + addCommas(jsonData[0].paretoFront[0]['cost($)'].toFixed(2)) + '</td></tr>');
      summaryTable.append('<tr><td>Purchased Part Cost:</td><td>$' + addCommas(purchasedPartCost.toFixed(2)) + '</td></tr>');
      summaryTable.append('<tr><td>Manufactured Part Cost:</td><td>$' + addCommas(manufacturedPartCost.toFixed(2)) + '</td></tr>');
      summaryTable.append('<tr><td>Assembly Cost:</td><td>$' + addCommas(jsonData[0].bestScore.assemblyDetails['cost($)'].toFixed(2)) + '</td></tr>');
      resultsWrapper.append(summaryTable);

      // Add The Purchased Parts
      if (purchasedCount != 0) {
        resultsWrapper.append('<h2>Purchased Parts</h2>');
        var purchasedTable = $('<table id="purchased-parts-table" />').addClass('table table-striped table-hover');
        purchasedTable.append('<thead><tr><th>Part</th><th class="sorttable_nosort"></th><th style="text-align: center;">Cost</th><th class="sorttable_nosort"></th><th class="sorttable_nosort"></th><th colspan=3 style="text-align: center;">Lead Time</th><th class="sorttable_nosort"></th></tr></thead>');
        resultsWrapper.append(purchasedTable);
      }

      // Add The Manufactured Parts
      if (manufacturedCount != 0) {
        resultsWrapper.append('<h2>Manufactured Parts</h2>');
        var manufacturedTable = $('<table id="manufactured-parts-table" />').addClass('table table-striped table-hover');
        manufacturedTable.append('<thead><tr><th>Part</th><th class="sorttable_nosort"></th><th style="text-align: center;">Cost</th><th class="sorttable_nosort"></th><th class="sorttable_nosort"></th><th colspan=3 style="text-align: center;">Lead Time</th><th class="sorttable_nosort"></th></tr></thead>');
        resultsWrapper.append(manufacturedTable);
      }

      // Loop Through The Data & Add To Parts Tables
      $.each(jsonData[0].bestScore.manufacturingDetails, function(index, part) {
        // Set Progress %
        if (part.type == "purchased") {
          var costProgress = purchasedPartCost == 0 ? 0 : (part['cost($)']   / purchasedPartCost * 100);
          var timeProgress = purchasedPartTime == 0 ? 0 : (part['time(min)'] / purchasedPartTime * 100);
        }

        if (part.type == "manufactured") {
          var costProgress = manufacturedPartCost == 0 ? 0 : (part['cost($)']   / manufacturedPartCost * 100);
          var timeProgress = manufacturedPartTime == 0 ? 0 : (part['time(min)'] / manufacturedPartTime * 100);
        }
        

        // Create Row Data
        var row = $(
          '<tr>' +
            '<td>' + part['name'] + '</td>' +
            '<td style="width: 80px; text-align: right;">$' + addCommas(part['cost($)'].toFixed(2)) + '</td>' +
            '<td sorttable_customkey="' + part['cost($)'] + '" style="width: 50px; text-align: right;">' + costProgress.toFixed(1) + '%</td>' +
            '<td style="width: 100px; text-align: left;">' +
              '<div class="progress progress-striped active">' +
                '<div class="progress-bar"  role="progressbar" aria-valuenow="' + costProgress.toFixed(0) + '" aria-valuemin="0" aria-valuemax="100" style="width: ' + costProgress.toFixed(0) + '%"></div>' +
              '</div>' + 
            '</td>' +
            '<td class="spacer-column"></td>' +
            '<td sorttable_customkey="' + part['time(min)'].toFixed(1) + '" style="width: 80px; text-align: right;">' + part['time(min)'].toFixed(2) + '</td>' +
            '<td style="width: 50px; text-align: right;">' + timeProgress.toFixed(1) + '%</td>' +
            '<td style="width: 100px; text-align: left;">' +
              '<div class="progress progress-striped active">' +
                '<div class="progress-bar"  role="progressbar" aria-valuenow="' + timeProgress.toFixed(0) + '" aria-valuemin="0" aria-valuemax="100" style="width: ' + timeProgress.toFixed(0) + '%"></div>' +
              '</div>' + 
            '</td>' +
            '<td class="spacer-column"></td>' +
          '</tr>'
        );

        // Add Row To Correct Table
        if (part.type == "purchased") {
          purchasedTable.append(row);
        } else {
          manufacturedTable.append(row);
        }
      });

      // Allow Tables To Sort
      sorttable.makeSortable(document.getElementById('purchased-parts-table'));
      sorttable.makeSortable(document.getElementById('manufactured-parts-table'));

      // Add The Gantt Chart
      resultsWrapper.append('<h2>Schedule</h2></br>');
      renderTimeline(resultsWrapper, jsonData[0]);
  });
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

function renderTimeline($parent, data) {
    // data
    var result = data.bestScore,
        items = [],
        // times
        startDate = new Date(result.assemblyDetails.startTime),
        endDate = new Date(result.assemblyDetails.completeTime),
        startTime = startDate.getTime(),
        endTime = endDate.getTime(),
        earliestStartTime = startTime,
        latestEndTime = endTime,
        totalDuration = null,
        // displayData
        $container = $('<div/>').
            addClass('ganttViewContainer');
    // prepare data
    $.each(result.manufacturingDetails, function () {
        var itemStartDate = new Date(this.startTime),
            itemEndDate = new Date(this.completeTime),
            itemStartTime = itemStartDate.getTime(),
            itemEndTime = itemEndDate.getTime();
        if (itemStartTime < earliestStartTime) {
            earliestStartTime = itemStartTime;
        }
        if (itemEndTime > latestEndTime) {
            latestEndTime = itemEndTime;
        }
        items.push({
            label: this.name,
            startTime: itemStartTime,
            endTime: itemEndTime
        });
    });
    items.push({
        label: "Assembly",
        startTime: startTime,
        endTime: endTime
    });
    items.unshift({
        label: "Total Lead Time",
        startTime: earliestStartTime,
        endTime: latestEndTime
    });
    totalDuration = latestEndTime - earliestStartTime;
    // build
    $.each(items, function (i) {
        // times
        var itemRelativeStartTime = this.startTime - earliestStartTime,
            itemDuration = this.endTime - this.startTime,
            dayDuration = itemDuration / (24 * 60 * 60 * 1000),
            dayLabel,
            // displayData
            itemLeftOffset = (itemRelativeStartTime / totalDuration) * 100 + '%',
            itemWidth = (itemDuration / totalDuration) * 100 + '%',
            // elements
            $itemContainer = $('<div/>').
                addClass('ganttViewItem').
                appendTo($container);
        dayLabel = ((dayDuration < 1) ? '<' : '~') + ' ' +
            Math.ceil(dayDuration) + ' ' +
            'day' + ((dayDuration > 1) ? 's' : '');
        $('<div/>').
            addClass('ganttViewItemBar').
            css({
                'left': itemLeftOffset,
                'width': itemWidth
            }).
            appendTo($itemContainer);
        $('<span/>').
            addClass('ganttViewItemLabel').
            html('<strong>' + this.label + '</strong> <em>( ' + dayLabel + ' )</em>').
            appendTo($itemContainer);
    });
    $parent.
        append('<h2>Timeline</h2>').
        append($container);
};