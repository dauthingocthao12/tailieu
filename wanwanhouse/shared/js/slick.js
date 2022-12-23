var sliderArray = {
    centerMode: true,
    slidesToShow: 1,
    infinite: true,
    arrows: true,
    // dots: true,
    centerPadding: 0,
    prevArrow: '<i class="gg-arrow-left-o"></i>',
    nextArrow: '<i class="gg-arrow-right-o"></i>',
    set_infinite: function (infinite) {
      this.infinite = infinite;
    },
    responsive: [{
        breakpoint: 1024,
        settings: {
          slidesToShow: 1,
        //   centerPadding: '50px'
        }
      }],
  };
$(document).ready(function(){
    $('.single-item').slick(sliderArray);
});
