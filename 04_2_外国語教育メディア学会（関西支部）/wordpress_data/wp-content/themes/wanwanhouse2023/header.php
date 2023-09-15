<!DOCTYPE html>
<html lang="en">

<head prefix="og: https://ogp.me/ns#">
	<meta charset="<?php bloginfo('charset'); ?>" />
	<meta name="viewport" content="width=device-width, initial-scale=1" />
	<meta property="og:url" content="https://wan-wan-house.com/" />
	<meta property="og:type" content=" website" />
	<meta property="og:title" content="<?php wp_title('', true, 'right'); ?><?php bloginfo('title'); ?>" />
	<meta property="og:description" content="<?php bloginfo('description'); ?>" />
	<meta property="og:site_name" content="<?php bloginfo('title'); ?>" />
	<meta property="fb:app_id" content="894466287315926" />
	<meta property="og:image" content="<?php echo get_template_directory_uri(); ?>/shared/images/common/wanwanhouse_ogp.jpg">
	<link rel="profile" href="https://gmpg.org/xfn/11" />
	<?php wp_head(); ?>
	<script src="<?php echo get_template_directory_uri(); ?>/shared/js/jquery-1.11.0.min.js"></script>
	<script src="<?php echo get_template_directory_uri(); ?>/shared/js/responsive.js?updt=20230324-4"></script>
	<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
	<link rel="stylesheet" href="<?= get_template_directory_uri() . "/slick-themes.css" ?>">
	<link rel="stylesheet" href="<?= get_template_directory_uri() . "/style.css?updt=20230324-4" ?>">
	<link rel="stylesheet" href="<?= get_template_directory_uri() . "/responsive.css?updt=20230324-4" ?>">
	<link rel="icon" href="<?= get_template_directory_uri() ?>/favicon.gif">
	<link rel="apple-touch-icon" href="<?= get_template_directory_uri() ?>/favicon_safari.jpg">
	<title><?php wp_title('|', true, 'right'); ?><?php bloginfo('title'); ?></title>
</head>

<body <?php body_class(); ?>>

	<header>
		<div class="header-container">
			<div class="header-logo-info-container">
				<h1 class="logo-image">
					<a href="<?php echo home_url() ?>" title="WAN WAN HOUSE トップ">
						<img src="<?php echo get_template_directory_uri(); ?>/shared/images/common/logo.png" alt="WAN WAN HOUSE" class="image-responsive">
					</a>
				</h1>

				<div class="header-shop-info">
					<i class="fa fa-home" aria-hidden="true"></i>前橋市岩神町3-12-19<br>
					<i class="fa fa-clock-o" aria-hidden="true"></i>午前9時から午後7時<br>
					<!-- <i class="fa fa-circle-o" aria-hidden="true"></i>毎週木曜日&nbsp;<i class="fa fa-phone-square" aria-hidden="true"></i><a href="tel:0272345400" class="tel">027-234-5400</a> -->
					<span class="free-day-icon"></span>毎週木曜日&nbsp;<i class="fa fa-phone-square" aria-hidden="true"></i><a href="tel:0272345400" class="tel">027-234-5400</a>
				</div>
			</div>


			<nav>
				<ul class="header-nav">
					<li><a href="<?php echo home_url() ?>">トップ</a></li>
					<li><a href="<?php echo home_url("trimming") ?>">トリミング</a></li>
					<li><a href="<?php echo home_url("shampoo-car") ?>">犬の移動美容室</a></li>
					<li><a href="<?php echo home_url("hotel") ?>">ドッグホテル</a></li>
					<li><a href="<?php echo home_url("pet-taxi") ?>">ペットタクシー</a></li>
					<li><a href="<?php echo home_url("net-shop") ?>">ネットショップ</a></li>
					<li><a href="<?= home_url("about") ?>" class="header-link">企業情報</a></li>
					<li><a href="<?= home_url("recruit") ?>" class="header-link">採用情報</a></li>
					<li><a href="<?= home_url(BLOG_SLUG) ?>" class="header-link">ブログ</a></li>
					<li><a href="<?= home_url("contact") ?>" class="header-link">お問合せ</a></li>
				</ul>
			</nav>
			<nav class="mobile-menu-container">
				<a href="tel:0272345400">
					<span class="fa-stack zoom-33">
						<i class="fa fa-circle-thin fa-stack-2x"></i>
						<i class="fa fa-phone fa-stack-1x"></i>
					</span>
				</a>
				<a href="#mobile-menu" class="mobile-menu-btn">
					<i class="fa fa-bars fa-2x"></i>
				</a>
				<div class="mobile-menu-pane">
					<!-- モバイルサブメニュー -->
					<div class="mobile-menu-close">
						<a href="#mobile-menu" class="mobile-menu-btn">
							<i class="fa fa-times fa-2x"></i>
						</a>
					</div>
					<section class="mobile-menu-hours">
						<dl>
							<div>
								<dt>住所</dt>
								<dd>〒371-0035<br>群馬県前橋市岩神町<br>3-12-19</dd>
							</div>
							<div>
								<dt>電話番号</dt>
								<dd>027-234-5400</dd>
							</div>
							<div>
								<dt>営業時間</dt>
								<dd>9:00～19:00</dd>
							</div>
							<div>
								<dt>定休日</dt>
								<dd>毎週木曜日</dd>
							</div>
						</dl>
					</section>

					<section class="mobile-menu-entries">
						<ul>
							<li class="_sep">
								<a href="<?php echo get_home_url(null, '/'); ?>">トップ</a>
							</li>

							<li class="_sep menu-list">
								<ul>
									<li>
										<a href="<?php echo home_url("trimming") ?>">トリミング</a>
									</li>
									<li>
										<a href="<?php echo home_url("shampoo-car") ?>">犬の移動美容室</a>
									</li>
									<li>
										<a href="<?php echo home_url("hotel") ?>">ドッグホテル</a>
									</li>
									<li>
										<a href="<?php echo home_url("pet-taxi") ?>">ペットタクシー</a>
									</li>
									<li>
										<a href="<?php echo home_url("net-shop") ?>">ネットショップ</a>
									</li>
								</ul>
							</li>

							<li class="_sep menu-list">
								<ul>
									<li>
										<a href="<?php echo home_url("about") ?>#shopinfor">企業情報</a>
									</li>
									<li>
										<a href="<?php echo home_url("about") ?>#access-map">アクセスマップ</a>
									</li>
								</ul>
							</li>

							<li class="_sep">
								<a href="<?php echo home_url("recruit") ?>" title="採用情報">採用情報</a>
							</li>

							<li class="_sep">
								<a href="<?php echo home_url(BLOG_SLUG) ?>" title="ブログ">ブログ</a>
							</li>

							<li class="_sep">
								<a href="<?php echo home_url("contact") ?>" title="お問合せ">お問合せ</a>
							</li>

							<li  class="sublink">
								<a href="<?php echo home_url("privacy-policy") ?>">プライバシーポリシー</a>
							</li>
						</ul>

						<div class="mobile-menu-social">
							<a href="https://www.facebook.com/%E3%83%AF%E3%83%B3%E3%83%AF%E3%83%B3%E3%83%8F%E3%82%A6%E3%82%B9-%E5%89%8D%E6%A9%8B-894466287315926" target="_blank" title="FACEBOOK">
								<span class="fa-stack">
									<i class="fa fa-circle fa-stack-2x"></i>
									<i class="fa fa-facebook fa-stack-1x text-secondary"></i>
								</span>
							</a>
							<span class="sep"></span>
							<a href="https://www.instagram.com/wanwanhouse/" target="_blank" title="INSTAGRAM">
								<i class="fa fa-instagram fa-2x"></i>
							</a>
						</div>
					</section>
				</div>
			</nav>
		</div>

		<div class="overlay"></div>
	</header>