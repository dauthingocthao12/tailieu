
//aタグがクリックされたらGoogleAnalyticsにクリックイベントを送信する 
//未使用なので止めます
// $("a").click(function(e) {
// 	var ahref = $(this).attr('href');
// 	var p = window.location.protocol;
// 	if(p == "https:"){
// 		ga('send', 'event', 'Aタグ', 'クリック', ahref);
// 	}
// });

function bookmark(siteurl, sitename, site_id, check) {
	var $ref = document.referrer;
	//alert(sitename);
	$('.sitename_mdl').empty();
	$('.sitename_mdl').append(sitename);
	$(".modalurl a").attr("href", siteurl);
	$(".modalurl a").on("click", function () {
		ga('send', 'event', 'site_link', 'click', 'site_id_' + site_id);
		del();
	});
	if ($.cookie("cookietest") || $ref == "" || check == "app_user") {
		//$.cookie("cookietest","",{path:"/",expires:-1});
		$('#url_mv_cnfrmtn').modal('show');
	} else {
		$.cookie("cookietest", "exist", { expires: 30, path: "/" })
		$('#bookmark').modal('show');
	}
}
function del() {
	$('#bookmark').modal('hide');
	$('#url_mv_cnfrmtn').modal('hide');
}
//七夕イベント背景の短冊切り替え
if ($('body').hasClass('tanabata_top') || $('body').hasClass('tanabata')) {
	$(window).scroll(function () {
		$(this).ready(function () {
			var tanabataflg = false;
			if ($(this).scrollTop() > 400) {
				//	$('.evbg_default').css('display','none');
				$('.evbg_default').hide();
				$('.evbg_change').show();
				tanabataflg = true;
			} else if ('tanabataflg') {
				$('.evbg_default').show();
				$('.evbg_change').hide();
			}
		});
	});
}
//メニューボタン
$(document).ready(function () {

	var usrid = $.cookie("user");
	if ($.cookie("user") === undefined) {
		usrid = "NO_LOGIN";
	}

	var menuOpen = false;
	function toggleMenu() {
		$(".menu-trigger").toggleClass("active");
		$(".menu-trigger").next().toggleClass("onanimation");
		menuOpen = $(".menu-trigger").hasClass("active");
		if (menuOpen) {
			makeMobileMenuFilter();
		}
		else {
			$('#mobileMenuFilter').remove();
		}
	}
	$(".menu-trigger").click(function (e) {
		toggleMenu();
		e.stopPropagation();
		return false;
	});
	$("body").click(function () {
		if (menuOpen) {
			toggleMenu()
		}
	});

	//フッターモーダル
	var data = {
		'action': 'footer-link',
		'date': $('#detail-page-link').data('date'),
		'data_type': $('#detail-page-link').data('data_type'),
		'star': $('#detail-page-link').data('star')
	};

	$.ajax({
		type: 'GET',
		url: '/footer-link-ajax.php',
		dataType: 'html',
		data: data,
		success: function (data) {
			// console.log(data);
			$('#detail-page-link').after(data);
		}
	});


	function makeMobileMenuFilter() {
		var filter = $("<div id='mobileMenuFilter'></div>");
		filter.css({
			'position': 'fixed',
			'z-index': 998,
			'top': 0,
			'left': 0,
			'right': 0,
			'bottom': 0,
			'background': 'rgba(0, 0, 0, 0.8)'
		});
		$('body').append(filter);
	}

	//トップページ説明文のトグル制御
	//collapseを表示
	$('.site-info').on('show.bs.collapse', function () {
		$('#uranai_info_abbr').removeClass("visible-xs").addClass("hide");
		$('#uranai_info_read_more').removeClass("visible-xs").hide();
		//GAに説明文を開いたことを送信
		if (window.ga) {
			ga('send', 'event', 'サイト説明文', 'click', 'user_' + usrid);
		}
	})
	//collapseを非表示
	$('.site-info').on('hide.bs.collapse', function () {
		$('#uranai_info_abbr').addClass("visible-xs").show();
		$('#uranai_info_read_more').addClass("visible-xs").show();
	})

	//運勢リンク提案エリア
	$('#unsei-suggest-link').on('click', function () {
		ga('send', 'event', '運勢提案リンク', 'click', $(this).attr('href'));
	});

	// バナーをクリックしてGoogleAnalyticsにクリック情報を送る
	$(".send-analytics").on('click', function () {
		ga('send', 'event', '猫の日のリンクバナー', 'click', $(this).attr('href'));
	});

	// それぞれの猫のリンクの情報を送信
	$(".send-season-event").on('click', function () {
		ga('send', 'event', $(this).attr('alt'), 'click', $(this).attr('href'));
	});

	// JSでリンク送信
	$('.news-link1').on('click', function(){
		window.open('https://www.jomo-news.co.jp/articles/-/302662', '_blank');
	})
	$('.news-link2').on('click', function(){
		window.open('https://prtimes.jp/main/html/rd/p/000000001.000123780.html', '_blank');
	})
	$('.news-link3').on('click', function(){
		window.open('https://chataibot.tech/', '_blank');
	})
});

// TOPへ >>>


$(function () {
	var showFlag = false;
	var topBtn = $('#page-top');
	var snsBtn = $('#sns_float_wrapper');
	var showFlag = false;

	//	function scroll() {
	//	var isScrolling = 0 ;
	//	var timeoutId ;
	//
	//	topBtn.stop().animate({'top' : '-100px'}, 200);
	//
	//	// スクロールを停止して500ms後に終了とする
	//	clearTimeout( timeoutId ) ;
	//
	//	timeoutId = setTimeout( function () {
	//		topBtn.stop().animate({'top' : '50px'}, 200);
	//	}, 600 ) ;
	//console.log('evevt');
	//}

	//スクロールが200に達したらボタン表示
	$(window).scroll(function () {
		if ($(this).scrollTop() > 200) {
			showFlag = true;
			topBtn.stop().animate({ 'top': '20px' }, 200);
			snsBtn.stop().animate({ 'top': '100px' }, 200);

			//window.addEventListener( "scroll",scroll) ;

		} else {
			//window.removeEventListener("scroll",scroll);
			showFlag = false;
			topBtn.stop().animate({ 'top': '-900px' }, 200);
			snsBtn.stop().animate({ 'top': '-900px' }, 200);
			$("#sns_btn_group").addClass("hidden");
		}
	});
	//スクロールしてトップ
	topBtn.click(function () {
		$('body,html').animate({
			scrollTop: 0
		}, 300);
		return false;
	});

	topBtn.mouseover(function () {
		return false;
	});

	topBtn.mouseout(function () {
		return false;
	});

});
// <<<


// details panels >>>
// all closed by default
//$('.panel-body').not(':first').hide();
//$('.panel-heading:first').addClass('open');

$('.panel-rank-details-sites .panel-heading').click(function () {
	if ($(this).hasClass('open')) { return; }
	var me = this;
	// data loaded already?
	if ($(this).next('.panel-body').find('ul').length == 1) {
		details_switch.apply(me);
	}
	else {
		var details_url = '/ajax-ranking-details.php?';
		details_url += 'date=' + $('#listing-ranking.ranking-details').data('details-date');
		details_url += '&star=' + $('#listing-ranking.ranking-details').data('details-star');
		details_url += '&rank=' + $(this).data('rank');

		$(this).append('<span class="label label-default">loading...</span>');
		$(this).next('.panel-body')
			.load(details_url, function () {
				details_switch.apply(me);
			});
	}
});


/**
	* switch open and closed detail panels
	*
	* @author Azet
	* @param jQueryObject header_
	*/
function details_switch() {
	var head = this;
	// closing any other open panel
	$('.open').next('.panel-body').slideUp(100,
		function () {
			$('html, body').animate({
				scrollTop: $(head).offset().top
			}, 200);
		}
	);

	$('.panel-heading').removeClass('open');
	$(this).next('.panel-body').slideDown();
	$(this).addClass('open');
	// remove the loading indicator
	$('.label', this).remove();
}
// <<<


/**
 * add okabe 2016/03/18
 * use from account-password-lost.tpl, account-delete.tpl
 *
 * @author Azet
 * @param none
 */
function input_madrs_check(a) {
	var email = document.form.email.value;
	$('.mail_miss,.mail_null').hide();
	if (email == '') {
		$('.mail_null').show();
		return false;
	}
	if (!email.match(/.*@.*\..*/i)) {
		$('.mail_miss').show();
		return false;
	}

	/* add okabe 2016/06/24 */
	if (a == 1) {
		$(".modal_text").text("パスワードの再発行を行なって宜しいですか?")
		$(alertModal).modal('show');
	} else {
		$(".modal_text").text("ユーザーを削除して宜しいですか?")
		$(alertModal).modal('show');
	}
	/*document.form.submit();*/
}

function modal_func() {
	document.form.submit();
}

/**
 * add okabe 2016/06/22
 */
function openRegistOverlay(v) {
	document.getElementById("ovlInfo").style.display = v;
	document.getElementById("ovl2Info").style.display = v;
}


/**
 * タグをフラッシュする
 *
 * @author Azet
 * @param jQuery セレクター $el_
 */
function flash($el_) {
	$el_.animate({ opacity: 0 }, 500);
	// $el_.delay(500);
	$el_.animate({ opacity: 1 }, 500);
}
/*年間・月間用*/
function past_year($year_) {
	//	$('#month').children().removeClass('show');
	//	$('#month').children(".m-"+$year_).addClass('show');
	$('.year-li').removeClass('current');
	$(".y-" + $year_).addClass('current');
	//	$(this).parent('li').addClass('OK');
	//	console.log($(this).parent('li'));
}

function sns_pop() {
	$("#sns_btn_group").toggleClass("hidden");
}

//紹介文トグルイベントリスナー追加
$(document).ready(function () {

	$('#uranai_info').on('show.bs.collapse', function () {
		$(this).removeClass("hidden-xs");
		// $(this).removeClass("visible-sm");

	}).on('hidden.bs.collapse', function () {
		$(this).addClass("hidden-xs");
		// $(this).addClass("visible-sm");
	});

});

function pick_ad(id) {
	$('#ad-preview input[name=ad-demo]').val(1);
	$('#ad-preview').append("<input type='hidden' name='ad_key' value='" + id + "'>");
	$('#ad-preview').submit();
}

function remove_ad_debugger() {
	$('#ad-preview-btn').remove();
	$('.adp-box').remove();
}

// イベント用モーダル画面 2023/02.20 add
$(document).ready(function () {

	$('.modal-start').each(function () {
		$(this).on('click', function () {
			$('.season-container').addClass('show');
			$('.mask').addClass('show');
			$('.modal-wrapper').addClass('show');
		});
	});

	// イベントバブリング対策
	$('.modal-wrapper').on('click', function (e) {
		e.stopPropagation();
	});


	$('.mask').on('click', function () {
		if ($('.mask').hasClass('show')) {
			$('.season-container').removeClass('show')
			$('.mask').removeClass('show');
			$('.modal-wrapper').removeClass('show');
		}
	});


	// モダール画面内の要素をランダムに並び替える // 20230406 off uenishi
	// function shuffleContent(container) {
	// 	var content = container.find('>*');
	// 	var total = content.length;
	// 	content.each(function () {
	// 		content.eq(Math.floor(Math.random() * total)).prependTo(container);
	// 	});
	// }

	$('.modal-start').on('click', function () {
		shuffleContent($('.season-links'))
	});
});