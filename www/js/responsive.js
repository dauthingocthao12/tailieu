$('document').ready(function () {

    // 左メニュー
    Responsive.init();

    // サイドバナーをスクロールさせる
    SideBanners.init();

    // TOPに戻るボタン
    TopArrow.init();



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
        $(window).resize(Responsive.handle);
        this.handle();
        $('#navigation-header').show();
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
        $('body').removeClass('mobile-menu-opened')
    }

};


// 上に戻るボタン
var TopArrow = {

    init: function() {
        var topArrow = jQuery('<a id="TopArrowScrollBtn" style="display: none;" href="#scroll"></a>');
        topArrow.click(this.doScroll);
        jQuery('body').append(topArrow);
        jQuery(document).scroll(this.checkScroll);
    },

    doScroll: function(e_) {
        e_.preventDefault();
        if(jQuery(this).attr('href')==='#scroll'){
            jQuery('body, html').animate({scrollTop: 0});
        }
    },

    checkScroll: function () {
        var topscroll = jQuery(document).scrollTop();
        if(topscroll>0) {
            jQuery('#TopArrowScrollBtn').fadeIn('fast');
        }
        else {
            jQuery('#TopArrowScrollBtn').fadeOut('fast');
        }
    }
};