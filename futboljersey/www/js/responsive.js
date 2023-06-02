$('document').ready(function () {

	// 左メニュー
	Responsive.init();

	// サイドバナーをスクロールさせる
	SideBanners.init();

	// TOPに戻るボタン
	TopArrow.init();

	Categories.init();

	// モーダル表示
	Modal.init();

	// if(location.pathname == "/") {
	//     $(#mobile-button-right).html("おすすめ情報");
	// }
	// else {
	//     $("mobile-button-right").html("お知らせ");
	// }
});

// サイドバナーの追従スクロール
SideBanners = {
	init: function () {
		window.addEventListener("scroll", SideBanners.scroll);
	},

	scroll: function () {
		var sidebarScroll = jQuery(document).scrollTop();
		var offset = sidebarScroll * -0.2;
		jQuery(".vertical-banner").css(
			"background-position",
			"center " + offset + "px"
		);
	}
};



// ヘッダーメニューのサイドバー表示
Responsive = {
	winWidth: 0,
	previousMode: '',
	previousMenuLeft: false,

	leftMenuTrigger: 949,

	init: function () {
		// サイズのイベント
		$(window).resize(Responsive.handle);
		// 基本サイズで起動(bootstrap)
		this.handle();
		this.addHeaderMenuHandle();

		$('.mobile-menu #hamburger-menu').click(this.mobileMenuToggle);
		$('#mobile-menu-overlay').click(this.mobileMenuClose);
	},

	viewport: function () {
		var e = window,
			a = 'inner';
		if (!('innerWidth' in window)) {
			a = 'client';
			e = document.documentElement || document.body;
		}

		return { width: e[a + 'Width'], height: e[a + 'Height'] };
	},

	handle: function () {
		var winWidth = Responsive.viewport().width;
		if (winWidth == Responsive.winWidth) {
			return;
		} else {
			Responsive.winWidth = winWidth;
		}

		var currentMode = 'pc',
			mobileMenuLeft = false;

		if (winWidth <= Responsive.leftMenuTrigger) {
			currentMode = 'mobile';
			mobileMenuLeft = true;
		}

		if (currentMode == "mobile") {
			$("#navigation-header").hide();
		}
		else {
			$("#navigation-header").show();
		}

		if (
			Responsive.previousMode != currentMode ||
			Responsive.previousMenuLeft != mobileMenuLeft
		) {
			if (currentMode == "mobile") {
				$("#navigation-mobile").append($("#login_name"));
				$("#navigation-mobile").append($("#navbox"));

				$('body').removeClass('pc').addClass('mobile');
			} else {
				$("#login-menu-pc").append($("#login_name"));
				$('body').removeClass('mobile').addClass('pc');
			}

			if (
				(currentMode == 'mobile' && mobileMenuLeft == false) ||
				currentMode == 'pc'
			) {
				$('#login-menu-pc').append($('#login_menu'));
				$('#pc-col-left').append($('#navbox'));
			}

			Responsive.mobileMenuClose();
			Responsive.previousMode = currentMode;
		}
	},

	addHeaderMenuHandle: function () {
		var newButton = $("<div class='btn-nav-header mobile-only'><button type='button'>メインメニュー</button></div>");
		$("#navigation-header").after(newButton);
		$("button", newButton).click(Responsive.headerMenuToggle);
	},

	headerMenuToggle: function () {
		$("#navigation-header").slideToggle();
	},

	mobileMenuToggle: function () {
		if ($('#navigation-mobile #navbox').is(':visible')) {
			Responsive.mobileMenuClose.call(this);
		}
		else {
			Responsive.mobileMenuOpen.call(this);
		}
	},

	mobileMenuOpen: function () {
		$('body').addClass('mobile-menu-opened');
	},

	mobileMenuClose: function () {
		$('body').removeClass('mobile-menu-opened');
	}

};


// 上に戻るボタン
var TopArrow = {

	init: function () {
		var topArrow = jQuery('<a id="TopArrowScrollBtn" style="display: none;" href="#scroll"></a>');
		topArrow.click(this.doScroll);
		jQuery('body').append(topArrow);
		jQuery(document).scroll(this.checkScroll);
	},

	doScroll: function (e_) {
		e_.preventDefault();
		if (jQuery(this).attr('href') === '#scroll') {
			jQuery('body, html').animate({ scrollTop: 0 });
		}
	},

	checkScroll: function () {
		var topscroll = jQuery(document).scrollTop();
		if (topscroll > 0) {
			jQuery('#TopArrowScrollBtn').fadeIn('fast');
		}
		else {
			jQuery('#TopArrowScrollBtn').fadeOut('fast');
		}
	}
};

var Categories = {
	init: function () {
		if ($(".categories .cate-1").get().length > 0) {
			$(".categories .cate-1").addClass("openable");
			$(".categories").each(function (x_) {
				Categories.encloseCate1(this, "cate-1");
			});
		}

		// lelvel 2 ある場合
		$(".categories .cate-2").addClass("openable");
//		$(".categories .cate-2").next("ul").hide();		初めから開いた状態にするためにコメントアウト　Nakajima
		$(".categories .cate-2").on("click", function (e_) {
			Categories.openSubGroup.call(this, e_);
		});
	},

	/**
	 * カテゴリー１のグループHTMLを準備する
	 * @param {Node} group_
	 */
	encloseCate1: function (group_, cate_) {
		var children = $(group_).children();
		// console.log( children ); // DBG
		var title = null;
		var content = [];

		children.each(function (x) {
			if ($(this).hasClass(cate_)) {
				title = this;
			}
			else {
				content.push(this);
			}
		});

		Categories._applyGrouping(title, content);
	},

	/**
	 * グループを作成、タイトルに開くアクションを設定
	 * @param {Node} title_
	 * @param {Node[]} content_
	 */
	_applyGrouping: function (title_, content_) {
		var container = $("<div class='cate-group'></div>");
		container.append(content_);
		$(title_).after(container);
		container.hide();
		$(title_).on("click", Categories.openGroup);
	},

	/**
	 * カテゴリーアクションのjQuery Handler
	 * @param {Event} e_
	 */
	openGroup: function (e_) {
		e_.preventDefault();
		$(this).toggleClass("opened");
		$(this).next(".cate-group").slideToggle('fast');
	},

	/**
	 * カテゴリーアクションのjQuery Handler
	 * @param {Event} e_
	 */
	openSubGroup: function (e_) {
		e_.preventDefault();
		$(this).toggleClass("opened");
		$(this).next("ul").slideToggle('fast');
	}
};


// モーダル表示
var Modal = {
	init: function () {

		// 画面幅500px以下で機能する
		if (window.matchMedia("(max-width: 550px)").matches) {
			//マーキングページであれば
			if ($("#caution_img").get().length > 0) {
				// 画像をクリックしたときにイベント発火
				// (imgタグのクラス名に"modal"を入れたらモーダル表示される)
				$('.modal').each(function () {
					$(this).on('click', function () {
						$('.modal-wrapper').addClass('show');
						$('.modal-image').addClass('show');

						var imageSrc = $(this).attr('src');
						$('.modal-image').attr('src', imageSrc);
					})
				});

				// 通常画面に戻る
				$('.modal-wrapper').on('click', function () {
					if ($(this).hasClass('show')) {
						$(this).removeClass('show');
						$('.modal-image').removeClass('show');
					}
				});
			};
		};
	}
};