/**
 * jQuery.fn.sortElements
 * --------------
 * @param Function comparator:
 *   Exactly the same behaviour as [1,2,3].sort(comparator)
 *
 * @param Function getSortable
 *   A function that should return the element that is
 *   to be sorted. The comparator will run on the
 *   current collection, but you may want the actual
 *   resulting sort to occur on a parent or another
 *   associated element.
 *
 *   E.g. $('td').sortElements(comparator, function(){
 *      return this.parentNode;
 *   })
 *
 *   The <td>'s parent (<tr>) will be sorted instead
 *   of the <td> itself.
 */
jQuery.fn.sortElements = (function(){

    var sort = [].sort;

    return function(comparator, getSortable) {

        getSortable = getSortable || function(){return this;};

        var placements = this.map(function(){

            var sortElement = getSortable.call(this),
                parentNode = sortElement.parentNode,

                // Since the element itself will change position, we have
                // to have some way of storing its original position in
                // the DOM. The easiest way is to have a 'flag' node:
                nextSibling = parentNode.insertBefore(
                    document.createTextNode(''),
                    sortElement.nextSibling
                );

            return function() {

                if (parentNode === this) {
                    throw new Error(
                        "You can't sort elements if any one is a descendant of another."
                    );
                }

                // Insert before flag:
                parentNode.insertBefore(this, nextSibling);
                // Remove flag:
                parentNode.removeChild(nextSibling);

            };

        });

        return sort.call(this, comparator).each(function(i){
            placements[i].call(getSortable.call(this));
        });

    };

})();

/**
 * ifabresults main script
 */
(function ($) {
    var roundValue = function (v) {
                if (v === null) {
                    return v;
                }
                return v.toFixed(2);
            },
            ratioPieOptions = {
                series: {
                    pie: {
                        show: true,
                        radius: 1,
                        label: {
                            show: false
                        }
                    }
                },
                legend: {
                    show: false
                }
            },
            renderTimeline = function($parent, data) {
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
    $(document).ready(function () {
        var $container = $('#results_container'),
                renderers = {

                    //
                    // Detailed Renderer
                    //
                    detailed: function (jsonData) {
                        var $purchasedContainer = $('<div/>').addClass('table-container'),
                                $manufacturedContainer = $('<div/>').addClass('table-container'),
                                $timelineContainer = $('<div/>').addClass('table-container'),
                                $purchasedTable,
                                $manufacturedTable,
                                manufacturedTotalCost = 0,
                                manufacturedTotalTime = 0,
                                purchasedTotalCost = 0,
                                purchasedTotalTime = 0,
                                partsList = jsonData[0].bestScore.manufacturingDetails;

                        $container.
                                append('<h1>Detailed Manufacturability Analysis</h1>').
                                append($purchasedContainer).
                                append($manufacturedContainer).
                                append($timelineContainer);
                        $purchasedTable = $('<table/>').
                                addClass('vf-table').
                                append('<thead><tr>' +
                                        '<th class="sortHeader" data-sort-attr="data-name">Part</th>' +
                                        '<th class="sortHeader" data-sort-attr="data-cost" data-sort-type="number" data-sort-default="-1" colspan="3">Cost</th>' +
                                        '<th class="sortHeader" data-sort-attr="data-time" data-sort-type="number" data-sort-default="-1" colspan="3">Lead Time (minutes)</th>' +
                                        '</tr></thead>');
                        $manufacturedTable = $('<table/>').
                                addClass('vf-table').
                                append('<thead><tr>' +
                                        '<th class="sortHeader" data-sort-attr="data-name">Part</th>' +
                                        '<th class="sortHeader" data-sort-attr="data-cost" data-sort-type="number" data-sort-default="-1" colspan="3">Cost</th>' +
                                        '<th class="sortHeader" data-sort-attr="data-time" data-sort-type="number" data-sort-default="-1" colspan="3">Lead Time (minutes)</th>' +
                                        '</tr></thead>');

                        $.each(partsList, function (i, part) {
                            if (part.type === 'purchased') {
                                purchasedTotalCost += Math.max(0, part['cost($)']);
                                purchasedTotalTime += Math.max(0, part['time(min)']);
                            } else {
                                manufacturedTotalCost += Math.max(0, part['cost($)']);
                                manufacturedTotalTime += Math.max(0, part['time(min)']);
                            }
                        });
                        $.each(partsList, function (i, part) {
                            var costRatio, timeRatio, $row;
                            if (part.type === 'purchased') {
                                costRatio = (part['cost($)']) ? part['cost($)'] / purchasedTotalCost : 0;
                                timeRatio = (part['time(min)']) ? part['time(min)'] / purchasedTotalTime : 0;
                            } else {
                                costRatio = (part['cost($)']) ? part['cost($)'] / manufacturedTotalCost : 0;
                                timeRatio = (part['time(min)']) ? part['time(min)'] / manufacturedTotalTime : 0;
                            }
                            $row = $('<tr class="sortRow" data-name="' +
                                    part.name.toLowerCase() +
                                    '" data-cost="' + part['cost($)'] +
                                    '" data-time="' + part['time(min)'] +
                                    '"/>').
                                    append('<th>' + part.name + '</th>').
                                    append('<td class="currencyCell">' + (roundValue(part['cost($)']) || '') + '</td>' +
                                            '<td class="percentCell">' + (costRatio * 100).toFixed(1) + '%</td>' +
                                            '<td class="progressCell"><progress value="' + costRatio + '" max="1"></progress></td>').
                                    append('<td class="floatCell">' + (roundValue(part['time(min)']) || '') + '</td>' +
                                            '<td class="percentCell">' + (timeRatio * 100).toFixed(1) + '%</td>' +
                                            '<td class="progressCell"><progress value="' + timeRatio + '" max="1"></progress></td>');
                            if (part.type === 'purchased') {
                                $row.appendTo($purchasedTable);
                            } else {
                                $row.appendTo($manufacturedTable);
                            }
                        });

                        $purchasedContainer.
                                append('<h2>Purchased Parts</h2>').
                                append($purchasedTable);
                        $manufacturedContainer.
                                append('<h2>Manufactured Parts</h2>').
                                append($manufacturedTable);

                        renderTimeline($container, jsonData[0]);
                    },

                    //
                    // Conceptual Renderer
                    //
                    conceptual: function (jsonData) {
                        var partTotalCost = 0,
                                partTotalTime = 0,
                                $hullContainer = $('<div/>').addClass('table-container'),
                                $partsContainer = $('<div/>').addClass('table-container'),
                                $hullTable,
                                $partsTable;

                        $container.
                                append('<h1>Conceptual Manufacturability Analysis</h1>').
                                append($hullContainer).
                                append($partsContainer);

                        // hull data
                        $hullTable = $('<table/>').
                                addClass('vf-table vf-table-spacious').
                                append('<thead><tr>' +
                                        '<th colspan="2"></th>' +
                                        '<th class="tag-build">Build</th>' +
                                        '<th class="tag-material">Material</th>' +
                                        '<th class="tag-total">Total</th>' +
                                        '</tr></thead>').
                                append('<tr>' +
                                        '<th>Cost</th>' +
                                        '<td id="hullTotalCostGraph"></td>' +
                                        '<td class="currencyCell tag-build">' +
                                        roundValue(jsonData.hull['Build Cost']) +
                                        '</td>' +
                                        '<td class="currencyCell tag-material">' +
                                        roundValue(jsonData.hull['Material Cost']) +
                                        '</td>' +
                                        '<td class="currencyCell tag-total">' +
                                        roundValue(jsonData.hull['Total Cost']) +
                                        '</td>' +
                                        '</tr>').
                                append('<tr>' +
                                        '<th>Time</th>' +
                                        '<td id="hullTotalTimeGraph"></td>' +
                                        '<td class="floatCell tag-build">' +
                                        roundValue(jsonData.hull['Build Time']) +
                                        '</td>' +
                                        '<td class="floatCell tag-material">' +
                                        roundValue(jsonData.hull['Material Lead Time']) +
                                        '</td>' +
                                        '<td class="floatCell tag-total">' +
                                        roundValue(jsonData.hull['Total Time']) +
                                        '</td>' +
                                        '</tr>');
                        $('<div/>').
                                addClass('icon-pie-chart').
                                appendTo('#hullTotalCostGraph').
                                plot([
                                    {label: "Build Cost", data: jsonData.hull['Build Cost'], color: '#deb42a'},
                                    {label: "Material Cost", data: jsonData.hull['Material Cost'], color: '#9ac6ed'}
                                ], ratioPieOptions);
                        $('<div/>').
                                addClass('icon-pie-chart').
                                appendTo('#hullTotalTimeGraph').
                                plot([
                                    {label: "Build Time", data: jsonData.hull['Build Time'], color: '#deb42a'},
                                    {label: "Material Lead Time", data: jsonData.hull['Material Lead Time'], color: '#9ac6ed'}
                                ], ratioPieOptions);

                        // parts data
                        $.each(jsonData.parts, function (i, part) {
                            partTotalCost += Math.max(0, part.cost);
                            partTotalTime += Math.max(0, part.leadtime);
                        });
                        $partsTable = $('<table/>').
                                attr('id', 'partsTable').
                                addClass('vf-table').
                                append('<thead><tr>' +
                                        '<th class="sortHeader" data-sort-attr="data-name">Part</th>' +
                                        '<th class="sortHeader" data-sort-attr="data-cost" data-sort-type="number" data-sort-default="-1" colspan="3">Cost</th>' +
                                        '<th class="sortHeader" data-sort-attr="data-time" data-sort-type="number" data-sort-default="-1" colspan="3">Lead Time</th>' +
                                        '</tr></thead>');
                        $.each(jsonData.parts, function (i, part) {
                            var costRatio = (part.cost) ?
                                            part.cost / partTotalCost : 0,
                                    timeRatio = (part.leadtime) ?
                                            part.leadtime / partTotalTime :
                                            0;
                            $('<tr class="sortRow" data-name="' +
                                    part.name.toLowerCase() +
                                    '" data-cost="' + part.cost +
                                    '" data-time="' + part.leadtime +
                                    '"/>').
                                    append('<th>' + part.name + '</th>').
                                    append('<td class="currencyCell">' + (roundValue(part.cost) || '') + '</td>' +
                                            '<td class="percentCell">' + (costRatio * 100).toFixed(1) + '%</td>' +
                                            '<td class="progressCell"><progress value="' + costRatio + '" max="1"></progress></td>').
                                    append('<td class="floatCell">' + (roundValue(part.leadtime) || '') + '</td>' +
                                            '<td class="percentCell">' + (timeRatio * 100).toFixed(1) + '%</td>' +
                                            '<td class="progressCell"><progress value="' + timeRatio + '" max="1"></progress></td>').
                                    appendTo($partsTable);
                        });


                        $hullContainer.
                            append('<h2>Hull</h2>').
                            append($hullTable);
                        $partsContainer.
                            append('<h2>Parts</h2>').
                            append($partsTable);
                    }
                };
        $.ajax({
            url: RESOURCE_URL,
            dataType: "text",
            success: function (resp) {
                var jsonData = JSON.parse(resp),
                // check for detailed or conceptual (test may need updating if format changes)
                        renderer = (Object.prototype.toString.call(jsonData) === '[object Array]') ? renderers.detailed : renderers.conceptual;
                renderer(jsonData);

                $('.sortHeader').bind({
                    'click': function (e) {
                        var $this = $(this),
                                $table = $this.closest('table'),
                                sortDir = $this.attr('data-sort-dir'),
                                sortAttr = $this.attr('data-sort-attr'),
                                sortType = $this.attr('data-sort-type'),
                                parse = (sortType === 'number') ?
                                        function (x) {
                                            return parseFloat($(x).attr(sortAttr)) || 0;
                                        } :
                                        function (x) {return $(x).attr(sortAttr);};
                        if (typeof sortDir === 'undefined' ||
                                sortDir === "0") {
                            sortDir =
                                    parseInt($this.attr('data-sort-default') ||
                                            1);
                        } else {
                            sortDir = parseInt(sortDir) * -1;
                        }
                        $table.
                                find('.sortHeader').
                                attr('data-sort-dir', 0);
                        $this.attr('data-sort-dir', sortDir);
                        $table.
                                find('.sortRow').
                                sortElements(function (a1, b1) {
                                    var a2 = parse(a1), b2 = parse(b1);
                                    return (a2 > b2 ? 1 :
                                            a2 < b2 ? -1 : 0) *
                                            sortDir;
                                });
                    }
                });
            }
        });
    });

})(jQuery);
