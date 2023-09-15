jQuery(document).ready(function() {
    // $('#slide').cycle({ 
    //     fx:      'turnDown', 
    //     delay:   -4000 
    // });
    $('#utilSearchToggle').on('click', function (e) {
        e.preventDefault();
       if($(".header_search-input").val()) {
            $('#utilSearchForm').submit();
       }
        $(".header_search-input").toggleClass("open");
    });

    Slider.init();
});

var Slider = {
    timeout: 5000,
    runtime: 2000,
    init: function() {
        var timeout = this.timeout;
        $('.slide:nth-child(1)').show();
        setTimeout(function() {
            $('.slide:nth-child(2)').addClass('slide-active');
            Slider.run();
        }, timeout);
    },

    run: function() {
        var timeout = this.timeout;
        var runtime = this.runtime;
        var next_index = 2;
        var active_index = 1;
        $('.slide').each(function (i) {
            if($(this).hasClass('slide-active')) {
                if(i == 0) {
                    active_index = $('.slide').length;
                }
                next_index = i + 2;
            }
        });

        if(next_index > $('.slide').length) {
            next_index = 1;
        }

        $('.slide-active').slideDown(runtime, function() {
            $('.slide').each(function (i) {
                if(!$(this).hasClass('slide-active')) {
                    $(this).css("display", "");
                }
            });
            setTimeout(function() {
                $('.slide-active').removeClass('slide-active');
                $('.slide:nth-child('+next_index+')').addClass('slide-active');
                Slider.run();
            }, timeout);
        });
    }
};


