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
(function ($) {
	TopArrow = {
		init: function () {
			// arrow itself
			var topArrow = $('<a id="TopArrowScrollBtn" style="display: none;" href="#scroll"><i class="fas fa-chevron-up"></i></a>');
			// button event
			topArrow.click(this.doScroll);

			// appending element to the page
			$('body').append(topArrow);

			// scroll event handler
			$(document).scroll(this.checkScroll);
		},


		// jQuery event
		doScroll: function (e_) {
			e_.preventDefault();
			// possible to have different buttons with different actions
			if ($(this).attr('href') === '#scroll') {
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
				$('body, html').animate({
					scrollTop: 0
				});
			}
		},


		// scroll check function
		// jQuery event
		checkScroll: function () {
			var topscroll = $(document).scrollTop();
			// console.log( topscroll ); // DBG
			if (topscroll > 0) {
				// show
				$('#TopArrowScrollBtn').fadeIn('fast');
			} else {
				// hide
				$('#TopArrowScrollBtn').fadeOut('fast');
			}
		}
	};
})(jQuery);