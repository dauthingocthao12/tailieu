<!DOCTYPE html>
<html lang="en">
<!-- itami debug -->
<head prefix="og: https://ogp.me/ns#">
	<meta charset="<?php bloginfo('charset'); ?>" />
	<meta name="viewport" content="width=device-width, initial-scale=1" />
	<meta property="og:url" content="http://www.let-kansai.org/htdocs/" />
	<meta property="og:type" content=" website" />
	<meta property="og:title" content="<?php wp_title('', true, 'right'); ?><?php bloginfo('title'); ?>" />
	<meta property="og:description" content="<?php bloginfo('description'); ?>" />
	<meta property="og:site_name" content="<?php bloginfo('title'); ?>" />
	<meta property="fb:app_id" content="" />
	<meta property="og:image" content="<?php echo get_template_directory_uri(); ?>/shared/images/common/wanwanhouse_ogp.jpg">
	<link rel="profile" href="https://gmpg.org/xfn/11" />
	<?php wp_head(); ?>
	<script src="<?php echo get_template_directory_uri(); ?>/shared/js/jquery-1.11.0.min.js"></script>
	<script src="<?php echo get_template_directory_uri(); ?>/shared/js/responsive.js?updt=20230324-4"></script>
	<script src="<?php echo get_template_directory_uri(); ?>/shared/js/search-menu.js"></script>
	<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.2/css/all.min.css">
	<!-- <link rel="stylesheet" href="<?= get_template_directory_uri() . "/style.css" ?>">
	<link rel="stylesheet" href="<?= get_template_directory_uri() . "/responsive.css?updt=20230324-4" ?>"> -->
	<link rel="icon" href="<?= get_template_directory_uri() ?>/favicon.gif">
	<link rel="apple-touch-icon" href="<?= get_template_directory_uri() ?>/favicon_safari.jpg">
	<script src="https://malsup.github.io/jquery.cycle.all.js"></script>
	<title><?php wp_title('|', true, 'right'); ?><?php bloginfo('title'); ?></title>
</head>

<body <?php body_class(); ?>>

	<header class="container">
		<div class="header-container">
		<ul class="utility">
            <li class="header_social-item">
                <ul class="header_social">
                        <li class="header_social-item">
                            <a href="" class="header_link" target="_blank">
                                <i class="fa-brands fa-facebook-f"></i>
                            </a>
                        </li>
                        <li class="header_social-item">
                            <a href="" class="header_link" target="_blank">
                                <i class="fa-brands fa-x-twitter" style="color: #ffffff;"></i>
                            </a>
                        </li>
                        <li class="header_social-item">
                            <a href="" class="header_link" target="_blank">
                                <i class="fa-brands fa-instagram"></i>
                            </a>
                        </li>
                </ul>
            </li>
             <li class="header_contact">
                <a class="header_link header_link_contact" href="">CONTACT US</a>
            </li>  
			<li class="header_search">
                <form id="utilSearchForm" class="header_form" action="" method="get">
                    <input class="header_search-input" type="search" placeholder="ページ内検索" name="query">
                    <button id="utilSearchToggle" class="header_link search-btn" type="button"><i class="fas fa-search"></i></button>
                </form>
            </li>
		</ul>

		<div class="header-content">
			<div class="top-slides">
				<div class="slide slide-image-1">
				</div>
				<div class="slide slide-image-2">
				</div>
				<div class="slide slide-image-3">
				</div>
			</div>
			<div class="main-title">
				<div class="d-flex text">
					<div class="logo-image text">
						<img src="<?php echo get_template_directory_uri(); ?>/shared/images/common/let-logo.png" alt="LET ロゴ">
					</div>
					<h1>外国語教育メディア学会　関西支部</h1>
				</div>
				<p class="text">Japan Association for Language Education and Technology, Kansai Chapter</p>
			</div>
			<nav id="main-menu">
				<ul class="content-container-md">
					<li>
						<a href="/login.php?url=%2F" title="ホーム" class="header-btn home">HOME</a>
					</li>
					<li>
						<a href="/member.php" title="研究大会" class="header-btn">研究大会</a>
					</li>
					<li>
						<a href="/conf.php" title="各種フォーム" class="header-btn">各種フォーム</a>
					</li>
					<li>
						<a href="/conf.php" title="支部研究大会" class="header-btn">支部研究大会</a>
					</li>
					<li>
						<a href="/conf.php" title="お問い合わせ" class="header-btn">お問い合わせ</a>
					</li>
				</ul>
			</nav>
		</div>

		<div class="overlay"></div>
	</header>