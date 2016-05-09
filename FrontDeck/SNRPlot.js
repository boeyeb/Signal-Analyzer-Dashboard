function PlotSNR(socket, container, snrtext1, snrtext2, flowControl,  shiftControl)
{
    Highcharts.setOptions({
        global: {
            useUTC: false
        }
    });

    return new Highcharts.Chart({
    title: {
        text: 'Signal to noise ratio',
        style: {
         color: '#222',
         textTransform: 'uppercase',
         fontSize: '20px'
      }
 },

    xAxis: {
        type: 'linear',
        tickPixelInterval: 100
    },
    yAxis: {
        title: { text: 'snr',
            style: {
                color: '#222'}
            },
        tickInterval: 2,
        min: 0,
        max: 40
    },
    plotOptions: {
        spline: {
            marker: {
                enabled: false
            },
            pointInterval: 1,
            pointStart: 0
        },
        line:{
             marker: {
                enabled: false
            },
        },
        scatter:{
            marker: { radius: 8}
        }

    },
    chart: {

        type: 'column',
        renderTo: container,
        events: {
            load: function(){
                var series = this.series;
                socket.onmessage = function(evt)
                {
                    var bPlot = $(flowControl).hasClass("active")?true:false;
                    var bShift = $(shiftControl).hasClass("active")?true:false;
                    var x;
                    var snrs=evt.data.split(',').forEach(function(antValue){
                        var kvs = antValue.split(':');
                        if(kvs && kvs[0] == 't' && kvs[1])
                            x = kvs[1];
                        if(kvs && kvs[0] !== 't' && kvs[1]){
                            if(kvs[0]==1){
                                if(kvs[1].indexOf("-1")> -1)
                                    $(snrtext1).text("radio read error");
                                else
                                    $(snrtext1).text(kvs[1]);
                            }
                            if(kvs[0]==2){
                                if(kvs[1].indexOf("-1")> -1)
                                    $(snrtext2).text("radio read error");
                                else
                                    $(snrtext2).text(kvs[1]);
                            }
                            if(bPlot){
                                var py = (( parseFloat(kvs[1]) > 0 ) ?  parseFloat(kvs[1]) : null);
                                series[parseInt(kvs[0])-1].addPoint([parseInt(x),py], true, bShift);
                            }
                    }
                    })
                }
                socket.onclose = function()
                {
                    console.log("wsSNR: closed");
                };
            }
        }
    },
    series: [
        {
            name: 'Stream #1',
            style: {color: '#E0E0E3'},
            color:"#00fff2",
            data: (function() {
                var data = [],
                    tick = 1,
                    i;
                for (i = -5; i <= 0; i++) {
                    data.push({
                        x: tick+i,
                        y: 1
                    });
                }
                return data;
            })()
        },
        {
            name: 'Stream #2',
            style: {color: '#E0E0E3'},
            color:"#48fb47",
            data: (function() {
                var data = [],
                    tick = 1,
                    i;
                for (i = -5; i <= 0; i++) {
                    data.push({
                        x: tick+i,
                        y: 2
                    });
                }
                return data;
            })()
        }
    ]
    })
}
