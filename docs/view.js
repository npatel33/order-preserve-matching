$(function(){
    var gridster = $(".gridster ul").gridster({
        widget_margins: [20, 20],
        max_cols: 4
    }).data('gridster');
    var workflowList = [
        'codeql-analysis.yml',
        'docker-chromium-stable.yml',
        'publish-production-tags.yml',
        'bw-compatibility.yml'
    ]

    setInterval(function(){
        $.each($(".gridster li"), function(i, data){
            gridster.remove_widget(data);
        });
    
        $.each(workflowList, function(i, data){
            console.log(data);
            $.ajax({
                type: 'GET',
                url: 'https://api.github.com/repos/browserless/chrome/actions/workflows/'+ data + '/runs',
                headers: {'Accept': 'application/vnd.github+json',
                            'Authorization': 'Bearer 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', 
                        },
                dataType: "json",
                async: false,
                success: function(data, statusText, xhr) {
                    console.log(data['workflow_runs']);
                    var color = NaN;
                    var conclusion = data['workflow_runs'][0]['conclusion'];
                    if( conclusion == 'success'){
                        color = "green"
                    }
                    else if(conclusion == 'failure'){
                        color = "red"
                    }
                    else {
                        color = "gray"
                    }
        
                    gridster.add_widget('<li class="new" style="background-color:'+ color + '">'+data['workflow_runs'][0]['name']+'</li>', 2,1);
                }
            });
        });
    }, 10000);



});
