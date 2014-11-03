var global_cache = {};
	global_cache.test_pass = false;
$(function(){
	$('.footer').height(function(index, height) {
		return window.innerHeight - $(this).offset().top;
	});

	$('.mode-box').click(function(){
		switch(this.id){
			case 'test':
				if( global_cache.test_pass ) return;
				var $loader = $('<div>', {class:"loader",id:"loader",text:'Test in progress...'})
				global_cache.test_pass = true;
				$(this).append($loader)
				window.setTimeout(function(){
				$.ajax({
					url: "test_results",
					context: this // the CB context, about time John
				}).done(function(res) {
					var flag = false;
					try {
						flag = JSON.parse( res ).data
					} catch(e) {
						flag = false;
					}
					global_cache.test_pass = flag;
					$('#loader').removeClass("loader").text( flag?"Pass":"Fail" ).addClass("test-success")
				});
				},5000);
			break;
			case 'tuner':
			case 'UAV':
			window.open(this.id);
		}
	});


});