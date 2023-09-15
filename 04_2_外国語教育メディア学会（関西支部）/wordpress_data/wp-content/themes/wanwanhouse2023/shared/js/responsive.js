// slick
var sliderArray = {
  fade:true,
  autoplay: true,
  autoplaySpeed: 5500,
//   speed:300,
  infinite: true,
  slidesToShow: 1,
  slidesToScroll: 1,
  arrows: true,
  dots: true,
  centerMode: true,
  pauseOnFocus: false,
  pauseOnHover: true,
  pauseOnDotsHover: false,
  prevArrow: '<div class="gg-arrow-left-o"></div>',
  nextArrow: '<div class="gg-arrow-right-o"></div>',
  set_infinite: function (infinite) {
    this.infinite = infinite;
  },
};


/**
 * animates scrolling to anchor tags
 * (see sample usage in README)
 *
 * depends on:
 * - jQuery
 */
var AnchorScroller = {
	scrollOffset: 0, // if needed, use that offset so the view is scrolled accordingly

	scrollTo: function(hash_) {
			try {
					var trgt_id = hash_.replace('#', '');
					var trgt = $('a[name='+trgt_id+']')[0];
					//console.log(trgt);
					var top_pos = $(trgt).offset().top - this.scrollOffset;
					$('body, html').animate({scrollTop: top_pos});
			}
			catch(e) {
			}
	},

	check: function() {
			// check after loading (called from document.ready event)
			if( window.location.hash ) {
					AnchorScroller.scrollTo( window.location.hash );
			}
	}
};


// mobile menu body scroll lock
var scrollOffsetBkp = 0;

$(document).ready(function () {
  // slick
  $('.top-slides').slick(sliderArray);

  // down arrow
//   $('.footer-nav .arrow, .mobile-menu-entries .arrow').on('click', function(e_) {
// 		// e_.preventDefault();
// 		$(this).toggleClass("open");
//   });

	// qa
	$(".qa-group dt").click(function(e_) {
		e_.preventDefault();
		$(this).parent().toggleClass("open");
	});

	// top submenu
	setTimeout(initTopSubmenu, 300);


	setTimeout(AnchorScroller.check, 500);
	// scroll on specific links actions (clicks)
	$('.qa-menu a').click(function(e_) {
		e_.preventDefault();
		AnchorScroller.scrollTo( $(this).attr('href') );
	});


	// mobile menu
	function toggleMobileMenu(e_) {
		e_.preventDefault();
		var offset = $(window).scrollTop();
		if($(".mobile-menu-pane").hasClass("open")) {
			// will close: unlock body and restore scroll
			// 1: unlock scroll
			$("body").removeClass("locked");
			// 2: restore scroll
			$(window).scrollTop(scrollOffsetBkp);
		}
		else {
			// will open: backup scrool
			scrollOffsetBkp = offset;
			// lock scroll
			$("body").addClass("locked");
		}

		$(".mobile-menu-pane").toggleClass("open");
	}
	$(".mobile-menu-btn, header .overlay").click(toggleMobileMenu);


	/** レスポンシブ時に、ヘッダーメニューに影をつける */
	var timeInfoDefaultPosition = 180;

	// function checkTimeInfoPosition() {
	// 	var newTimeInfoPosition = $(".time-information p:first")[0].getBoundingClientRect().top;
	// 	console.log( newTimeInfoPosition ); // DBG
	// 	if(newTimeInfoPosition > 0 && newTimeInfoPosition > timeInfoDefaultPosition) {
	// 		setTimeout(checkTimeInfoPosition, 300);
	// 	}
	// 	else {
	// 		timeInfoDefaultPosition = newTimeInfoPosition;
	// 	}
	// }
	// checkTimeInfoPosition();

	var previousScroll = 0;

	$(window).scroll(function() {
		// トップヘッダー
		// if($(window).scrollTop() > 0) {
		// 	$("header").addClass("scrolled");
		// }
		// else {
		// 	$("header").removeClass("scrolled");
		// }

		var newScroll = $(window).scrollTop();

		if(newScroll > previousScroll && newScroll>0) {
			$("header").addClass("scrolled");
		}
		else {
			$("header").removeClass("scrolled");
		}

		previousScroll = newScroll;

		// 営業情報固定対応
		if($(window).scrollTop() > timeInfoDefaultPosition) {
			$(".time-information").addClass("fixed");
		}
		else {
			$(".time-information").removeClass("fixed");
		}
	});

	/** 上に戻る **/
	TopArrow.init();

	/** ページのアンカーにスクロール */
	// scroll after page loaded
	AnchorScroller.check();

	// scroll on specific links actions (clicks)
	$('a[href^="#"]').click(function(e_) {
		e_.preventDefault();
		AnchorScroller.scrollTo( $(this).attr('href') );
	});

});


/**
 * ヘッダーのトップメニュー（PC版）のドロップダウンのイベントを登録
 */
function initTopSubmenu() {
	$(".header-nav .has-submenu").on("click", function(e_) {
		$(this).toggleClass("open");
	});
	$(".header-nav .has-submenu").on("mouseenter", function(e_) {
		$(this).addClass("open");
	});
	$(".header-nav .sub-menu").on("mouseleave", function(e_) {
		$(this).closest(".has-submenu").removeClass("open");
	});
}


/**
 * animates scrolling to anchor tags
 * depends on:
 * - jQuery
 */
var AnchorScroller = {
	scrollOffset: 64, // if needed, use that offset so the view is scrolled accordingly

	scrollTo: function (hash_) {
		try {
			var trgt_id = hash_.replace('#', '');
			var trgt = $('a[name=' + trgt_id + ']')[0];
			//console.log(trgt);
			var top_pos = $(trgt).offset().top - this.scrollOffset;
			$('body, html').animate({ scrollTop: top_pos });
		}
		catch (e) {
		}
	},

	check: function () {
		// check after loading (called from document.ready event)
		if (window.location.hash) {
			AnchorScroller.scrollTo(window.location.hash);
		}
	}
};

/**
 * top arrow (scrool to top)
 * depends on:
 * - jQuery
 *
 * usage:
 * - TopArrow.init();
 *
 * tested on:
 * - IE 9,11
 * - Firefox 49
 * - Edge
 * - PC Chrome 53
 * - iPad (iOS 9)
 * - Android 7 Chrome
 */
var TopArrow = {
	init: function() {
			// arrow itself
			var topArrow = $('<a id="TopArrowScrollBtn" style="display: none;" href="#scroll"><i class="fa fa-chevron-up"></i></a>');
			// button event
			topArrow.click(this.doScroll);

			// appending element to the page
			$('body').append(topArrow);

			// scroll event handler
			$(document).scroll(this.checkScroll);
	},


	// jQuery event
	doScroll: function(e_) {
			e_.preventDefault();
			// possible to have different buttons with different actions
			if($(this).attr('href')==='#scroll') {
					//var offset = ($(document).scrollTop());
					//if(/(IE 9|Trident)/.test(navigator.userAgent)) {
					//  // IE 9,11 only
					//  $('body, html').animate({scrollTop: '-'+offset+'px'});
					//}
					//else if(/Firefox/.test(navigator.userAgent)) {
					//  $('body, html').animate({scrollTop: 0});
					//} else {
					//  // others
					//  //$('body').animate({scrollTop: '-'+offset+'px'});
					//}
					$('body, html').animate({scrollTop: 0});
			}
	},


	// scroll check function
	// jQuery event
	checkScroll: function () {
			var topscroll = $(document).scrollTop();
			if(topscroll>0) {
					// show
					$('#TopArrowScrollBtn').fadeIn('fast');
			}
			else {
					// hide
					$('#TopArrowScrollBtn').fadeOut('fast');
			}
	}
};