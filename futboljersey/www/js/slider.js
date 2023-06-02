// ***** slick 1.8.1 *****
$('document').ready(function () {

    const sliderPage = $('#slider');

    const noImage = $('.slider').children('img');

    const mediaQuery = 769;

    // 商品詳細ページなら
    if (sliderPage && !noImage.hasClass('noimage')) {

        // メインスライダー
        $('.slider').slick({
            autoplay: false,
            arrows: true,
            speed: 500,
            infinite: false,
            slidesToShow: 1,
            waitForAnimate: false,
            variableWidth: false,
            centerMode: false,
            dots: true,
            prevArrow: '<img src="/img_responsive/slider_arrow_up.png" class="prev-arrow nav-btn">',
            nextArrow: '<img src="/img_responsive/slider_arrow_down.png" class="next-arrow nav-btn">',

            // レスポンシブ対応
            responsive: [{
                breakpoint: 769,
                settings: {
                    touchThreshold: 6,
                    arrows: false,
                },
            }],
        });


        // 画面読み込み時にイベント発火
        let currentWidth = $(window).width();
        // スマホ画面
        if (currentWidth < mediaQuery) {

            // モーダル表示機能のオフ
            $('.modal').off('click');

        }
        // PC画面
        else {
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
        }

        // 画面幅の可変時に同様のイベントを発火させる。
        $(window).resize(function () {

            let mobileWidth = $(window).width();

            // スマホ画面
            if (mobileWidth < mediaQuery) {

                // モーダル表示機能のオフ
                $('.modal').off('click');

            }
            // PC画面
            else {
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
            }
        });

        return;

    }

    if (noImage.hasClass('noimage')) {

        // noimageを中央配置
        $('.carousel-section').css({ "margin": "auto" });

        return;

    }
});



