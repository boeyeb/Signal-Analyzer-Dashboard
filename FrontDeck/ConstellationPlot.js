var nstream = 0, qam0, qam1, nsample;

var stream0 = {
    chart: {
        type: 'bubble',
        plotBorderWidth: 1,
        renderTo: 'canvas0',
        zoomType: 'xy',
        marginRight: 80,
    },
    title:{
        text:'QAM'
    },
    xAxis: {
        min : -2048,
        max : 2048,
        plotLines:[{
            value:0,
            color:'#666',
            width:1
        }],
        gridLineWidth:1,
    },
    yAxis: {
        min : -2048,
        max : 2048,
        plotLines:[{
            value:0,
            color:'#666',
            width:1
        }],
        gridLineWidth:1,
    },
    plotOptions: {
           series: {
            animation: false
        },
        bubble: {minSize: 1,maxSize: 1},
    },
    series: [{name:'Stream #1', data: [],color:"#00fff2"}]
};

var stream1 = {
    chart: {
        type: 'bubble',
        plotBorderWidth: 1,
        renderTo: 'canvas1',
        zoomType: 'xy',
        marginRight: 80,
    },
    title:{
        text:'QAM'
    },
    xAxis: {
        min : -2048,
        max : 2048,
        plotLines:[{
            value:0,
            color:'#666',
            width:1
        }],
        gridLineWidth:1,
    },
    yAxis: {
        min : -2048,
        max : 2048,
        plotLines:[{
            value:0,
            color:'#666',
            width:1
        }],
        gridLineWidth:1,
    },
    plotOptions: {
           series: {
            animation: false
        },
        bubble: {minSize: 1,maxSize: 1},
    },
    series: [{name:'Stream #2',data: [], color:"#48fb47"}]
};

function sdPlot(data, canvas0, canvas1, canvas2, canvas3)
{
    var pkts = data.split('=');
    if(pkts[0].indexOf('HDR') >-1 )
    {
        var strs = pkts[1].split(',');
        nstream = parseInt(strs[0]);
        qam0 = parseInt(strs[1]);
        qam1 = parseInt(strs[2]);
        qam2 = parseInt(strs[3]);
        qam3 = parseInt(strs[4]);
        nsample = parseInt(strs[5]);
        console.log('HDR:[' + nstream + ',' + qam0 + ',' + qam1 +']');
        console.log(strs);
        stream0.title.text = 'QAM ' + strs[1];
        stream1.title.text = 'QAM ' + strs[2];
        if(qam0 == 4)
            stream0.xAxis.tickInterval = stream0.yAxis.tickInterval = 2048;
        if(qam0 == 8 || qam0 == 16)
            stream0.xAxis.tickInterval = stream0.yAxis.tickInterval = 1024;
        if(qam0 == 32|| qam0 == 64)
            stream0.xAxis.tickInterval =  stream0.yAxis.tickInterval = 512;
        if(qam0 == 128 || qam0 == 256)
            stream0.xAxis.tickInterval =  stream0.yAxis.tickInterval = 256;
        if(qam0 == 512|| qam0 == 1024)
            stream0.xAxis.tickInterval = stream0.yAxis.tickInterval = 128;

         if(qam1 == 4)
            stream1.xAxis.tickInterval = stream1.yAxis.tickInterval = 2048;
        if(qam1 == 8 || qam1 == 16)
            stream1.xAxis.tickInterval = stream1.yAxis.tickInterval = 1024;
        if(qam1 == 32|| qam1 == 64)
            stream1.xAxis.tickInterval =  stream1.yAxis.tickInterval = 512;
        if(qam1 == 128 || qam1 == 256)
            stream1.xAxis.tickInterval = stream1.yAxis.tickInterval = 256;
        if(qam1 == 512|| qam1 == 1024)
            stream1.xAxis.tickInterval = stream1.yAxis.tickInterval = 128;

        if(nstream == 1)
        {
            $("#sidebyside").hide();
            $("#upanddown").show();
        }

        if((nstream == 2) && ((qam0 == 512 || qam0 == 1024 || qam1 == 512 || qam1 == 1024)))
        {
            $("#sidebyside").hide();
            $("#upanddown").show();
        }

        if((nstream == 2) && (qam0 < 512) && (qam1 < 512))
        {
            $("#sidebyside").show();
            $("#upanddown").hide();
        }

    };

    if(pkts[0].indexOf('s1') >-1 )
    {

        var count = 0;
        var x, y;
        var sample = [];
        var xy = pkts[1].split(',');
        _.each(xy, function(m){
            if(count%2 == 0) {
                x = parseInt(m);
            }
            else {
                y =  parseInt(m);
                sample.push([x,y,1]);
            }
            count++;
        })
        if($("#upanddown").is(":visible"))
            stream0.chart.renderTo = canvas2;
        else
            stream0.chart.renderTo = canvas0;
        stream0.series[0].data = sample;
        var chart0 = new Highcharts.Chart(stream0);
    }

    if(pkts[0].indexOf('s2') >-1 )
    {
        var count = 0;
        var x, y;
        var sample = [];
        var xy = pkts[1].split(',');
        _.each(xy, function(m){
            if(count%2 == 0) {
                x = parseInt(m);
            }
            else {
                y =  parseInt(m);
                sample.push([x,y,1]);
            }
            count++;
        })
        if($("#upanddown").is(":visible"))
            stream1.chart.renderTo = canvas3;
        else
            stream1.chart.renderTo = canvas1;
        stream1.series[0].data = sample;
        var chart1 = new Highcharts.Chart(stream1);
    }
}
