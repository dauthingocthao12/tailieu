<!DOCTYPE html>
<!--[if lt IE 7]>      <html class="no-js lt-ie9 lt-ie8 lt-ie7"> <![endif]-->
<!--[if IE 7]>         <html class="no-js lt-ie9 lt-ie8"> <![endif]-->
<!--[if IE 8]>         <html class="no-js lt-ie9"> <![endif]-->
<!--[if gt IE 8]><!-->
<html class="no-js">
<!--<![endif]-->

<head>
	<!-- Google Tag Manager -->
	{literal}
		<script>
			(function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({'gtm.start':
			new Date().getTime(), event: 'gtm.js'
			});
			var f = d.getElementsByTagName(s)[0],
				j = d.createElement(s),
				dl = l != 'dataLayer' ? '&l=' + l : '';
			j.async = true;
			j.src =
				'https://www.googletagmanager.com/gtm.js?id=' + i + dl;
			f.parentNode.insertBefore(j, f);
			})(window, document, 'script', 'dataLayer', 'GTM-NWDDK6C');
		</script>
	{/literal}
	<!-- End Google Tag Manager -->
	<meta charset="utf-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	{block name="seo"}
		<title>星占い 12星座占いランキング</title>
		<meta name="keywords" content="12星座占いランキング,占い,ランキング">
		<meta name="description" content="各星占いのサイトから情報を取得して12星座占いのランキングを表示しております。">
		<meta name="twitter:card" content="summary_large_image">
		<meta name="twitter:site" content="@UranaiRankingJp" />
	{/block}
	<!--OGP START-->
	{include file="ogp.tpl" title="随時更新中！" des="各星占いのサイトから情報を取得して12星座占いのランキングを表示しております。"}
	<!--OGP END-->
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<!-- Place favicon.ico and apple-touch-icon.png in the root directory -->
	<link rel="stylesheet" href="/user/css/normalize.css">
	<link rel="stylesheet" href="/user/css/main.css">
	<link rel="stylesheet" href="/user/packages/bootstrap-3.1.1-dist/css/bootstrap.min.css">
	<link rel="stylesheet" href="/user/packages/font-awesome-4.5.0/css/font-awesome.min.css">
	<!--app things-->
	{if $design_name.class_name == "designA"}
		<link rel="stylesheet" href="/user/css/app-main.css?var={$config.cache_date}">
	{else}
		<link rel="stylesheet"
			href="/user/css/{if $design_name}{$design_name.ev_name}{/if}app-main.css?var={$config.cache_date}">
	{/if}
	{* modal window css *}
	<link rel="stylesheet" href="/user/css/season_event.css">
	<!-- style override for nav.navbar etc. -->
	{literal}
		<!-- ユーザーヒート -->
		<script type="text/javascript">
			(function(add, cla){window['UserHeatTag']=cla;window[cla]=window[cla]||function(){(window[cla].q=window[cla].q||[]).push(arguments)},window[cla].l=1*new Date();var ul=document.createElement('script');var tag = document.getElementsByTagName('script')[0];ul.async=1;ul.src=add;tag.parentNode.insertBefore(ul,tag);})('//uh.nakanohito.jp/uhj2/uh.js', '_uhtracker');_uhtracker({id:'uhlHJgq9kn'});
		</script>
		<!-- ユーザーヒート終了 -->
	{/literal}

	<script src="/user/js/vendor/jquery-1.12.0.min.js"></script>
	<script src="/user/js/vendor/jquery.cookie-1.4.1.min.js"></script>
	<!--カレンダー-->
	{block name="script"}
	{/block}

	{literal}
		<!-- adsense 新しくしたもの 2022/11 -->
		<script async src="https://pagead2.googlesyndication.com/pagead/js/adsbygoogle.js?client=ca-pub-6842258632348915"
			crossorigin="anonymous"></script>
	{/literal}

</head>
<!--
<body class="{if $data.extpara=='demo'}designA{else}{if $data.star_name_en}{$data.star_name_en}{else}{if $data.rank[0].en_name}{$data.rank[0].en_name}{else}{if $data.m_rank[0].en_name}{$data.m_rank[0].en_name}{else}{if $data.extpara=='whatnew'}whatnew{else}default{/if}{/if}{/if}{/if}{/if}">
-->

<body
	class="{if $design_name}{$design_name.class_name}{else}designA{/if} {if $data.star_name_en}{$data.star_name_en}{/if}">
	{include file="ogp.tpl" x="OK"}
	{if $smarty.server.REMOTE_ADDR != '127.0.0.1'}
		{include file="google_analytics.tpl"}
	{/if}

	{if !$notfoundpage}
		<div id="fb-root"></div>
		{literal}
			<script>
				(function(d, s, id) {
					var js, fjs = d.getElementsByTagName(s)[0];
					if (d.getElementById(id)) return;
					js = d.createElement(s);
					js.id = id;
					js.async = true;
					js.src = "//connect.facebook.net/ja_JP/sdk.js#xfbml=1&version=v2.8";
					fjs.parentNode.insertBefore(js, fjs);
				}(document, 'script', 'facebook-jssdk'));
			</script>

			<script>
				var timer = false;
				$(window).resize(function() {
					if (timer !== false) {
						clearTimeout(timer);
					}
					timer = setTimeout(function() {
						boxWidth = $('#fb-plugin').width();
						currentWidth = $('#fb-plugin .fb-page').attr('data-width');
						if (boxWidth != currentWidth) {
							$('#fb-plugin .fb-page').attr('data-width', boxWidth);
							FB.XFBML.parse(document.getElementById('pagePlugin'));
						}
					}, 200);
				});
			</script>
		{/literal}
	{/if}

	<!--[if lt IE 7]>
        <p class="browsehappy">You are using an <strong>outdated</strong> browser. Please <a href="http://browsehappy.com/">upgrade your browser</a> to improve your experience.</p>
    <![endif]-->

	<!-- Add your site or application content here -->
	{block name="ranking_head"}
		{if isset($design_name.ev_bg)}
			{$design_name.ev_bg}
		{/if}


		<div class="container ">
			{* イベント用モーダル *}
			{if $event_data["event_key"] == ""}
				<div class="season-container">
					<div class="mask">
						<div class="modal-wrapper mw-theme mw-size mw-shape">
							<h5 class="content-title">自分の星座をクリック！</h5>
							<div>
								<ul class="season-links">
									{foreach from=$modal_data item=value }
										{$value}
									{/foreach}
								</ul>
							</div>
							{* イベント用広告 *}
							<div class="banner-ads">
								{insert ad_group id="10"}
							</div>
						</div>
					</div>
				</div>
			{/if}


			<div class="head">
				<div class="row {if !$config.isApp}title-box{/if}">
					<div class="sitename float-l font-color">
						<h1 class="main-title"><a href="/" title="星占い 12星座占いランキング">今日の12星座占いランキング</a></h1>
					</div>
					{if !$hideLoginBtn}
						{insert loginbtn}
					{/if}
				</div>
				<!--{$generateDateMenu}-->
				<!--ナビバー-->
				<div class="row hidden-xs">
					<nav class="navbar navbar-default ">
						<div class="container">
							<div class="collapse navbar-collapse" id="navbarEexample">
								<a class="navbar-brand" href="/" title="星占い 12星座占いランキング">TOPへ</a>
								<ul class="nav navbar-nav">
									{if !$notfoundpage}
										<li class=" dropdown">
											<a class="dropdown-toggle" role="button" aria-expanded="false"
												data-toggle="dropdown">各星座<span style="margin-left:7px;"
													class="caret"></span></a>
											<ul class="dropdown-menu" role="menu">
												{section name=cnt start=1 loop=13}
													{assign var="xxx" value="star`$smarty.section.cnt.index`"}
													<li><a href="{if $data.date_num == date('Ymd')}{sitelink mode="{if $data.data_type != ''}{$data.data_type}/{/if}{$allEnStars[$smarty.section.cnt.index]}"
												}{else}{sitelink mode="{if $data.data_type != ''}{$data.data_type}/{/if}{$data.date_num}/{$allEnStars[$smarty.section.cnt.index]}"}{/if}">{$allJpStars[$xxx]}</a>
													</li>
												{/section}
											</ul>
										</li>
									{/if}
									<li><a href="{sitelink mode="{$data.link_basic.ranking_past}" }">年間・月間</a></li>
									{block name="nav_topic"}
										{if $data.link_basic.defolt !="404"}
											<li class="text-center"><a href="{sitelink mode="{$data.link_basic.defolt}" }"><i
														class="fa fa-star" aria-hidden="true"></i><span
														class="icon-title">総合運</span></a></li>
										{/if}
										{if $data.link_basic.love !="404"}
											<li class="text-center"><a href="{sitelink mode="{$data.link_basic.love}" }"><i
														class="fa fa-heart" aria-hidden="true"></i> <span
														class="icon-title">恋愛運</span></a></li>
										{/if}
										{if $data.link_basic.work !="404"}
											<li class="text-center"><a href="{sitelink mode="{$data.link_basic.work}" }"><i
														class="fa fa-pencil" aria-hidden="true"></i> <span
														class="icon-title">仕事運</span></a></li>
										{/if}
										{if $data.link_basic.money !="404"}
											<li class="text-center"><a href="{sitelink mode="{$data.link_basic.money}" }"><i
														class="fa fa-jpy" aria-hidden="true"></i> <span
														class="icon-title">金運</span></a></li>
										{/if}
										<li><a href="{sitelink mode="{$data.link_basic.digest}"
										}">{if $smarty.now|date_format:'%Y' == '2023'}今年{else}来年{/if}の運勢</a></li>
									{/block}
								</ul>
							</div>
						</div>
					</nav>
				</div>

				<!--ナビバー-->
				<!--パンくずリスト-->
				<ol class="breadcrumb" itemscope itemtype="http://schema.org/BreadcrumbList">
					{$Breadcrumblist}
				</ol>
				<!--パンくずリスト-->
			</div>
		{/block}
		<div class="body">
			{block name="body"}{/block}
			<div id="detail-page-link" data-date="{$data.date_num}" data-data_type="{$data.data_type}"
				data-star="{$data.star_name_en}">
			</div>
		</div>
	</div>
	<footer>
		<div class="navbar">
			<div class="container-fluid">
				<ul class="nav navbar-nav">
					<li><a href="{sitelink mode="whatnew"}">新着情報一覧</a></li>
					<li><a href="{sitelink mode="about"}">{$config.plateform}について</a></li>
					<li><a href="{sitelink mode="company"}">運営会社概要</a></li>
					<li class="divider"></li>
					<li><a href="{sitelink mode="kiyaku"}">利用規約</a></li>
					<li><a href="{sitelink mode="policy"}">プライバシーポリシー</a></li>
					<li><a href="{sitelink mode="site-list"}">サイト一覧</a></li>
					<li><a href="{sitelink mode="howtouse"}">{$config.plateform}の使い方</a></li>
					<li><a href="{sitelink mode="contact"}">お問合わせ</a></li>
				</ul>

				{if !$notfoundpage}
					{* SNSシェアボタン サイトスピード改善の為レンダリングしないように。
                <ul class="nav navbar-nav navbar-right social">
                    <li><div class="fb-share-button" data-href="http://www.uranairanking.jp/" data-layout="button_count"></div></li>
                    <li>
                    <a href="https://twitter.com/share" class="twitter-share-button"></a>
{literal}<script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.async = true;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script>{/literal}
                    </li>
                </ul>
*}
				{/if}

			</div>
		</div>
	</footer>

	<!--{$generateDateMenu}-->
	<!-- scripts at the end -->
	<script src="/user/js/vendor/modernizr-2.6.2.min.js?var={$config.cache_date}"></script>
	<script src="/user/js/uranai.js?var={$config.cache_date}"></script>
	<script src="/user/packages/bootstrap-3.1.1-dist/js/bootstrap.min.js?var={$config.cache_date}"></script>

	<div class="reset" id="page-top"><a href="#wrap"><img src="/user/img_re/top_back.png" alt="上に戻る"></a></div>

	<div id="sns_float_wrapper">
		<div id="sns_float">
			<a class="sns-btn-float-base" id="sns_opner" href="javascript:void(0);" onclick="sns_pop();"><img
					src="/user/img_re/sns-follow.png">
			</a>
			<div id="sns_btn_group" class="hidden">
			<a class="sns-btn-float" onclick="ga('send','event','visit_sns','click','facebook');" target="_blank"
				id="fb_float" href="https://www.facebook.com/12&#x661f;&#x5ea7;&#x5360;&#x3044;&#x30e9;&#x30f3;&#x30ad;&#x30f3;&#x30b0;-517659478433475"><img src="/user/img_re/f_logo.png" alt="facebookへ">
			</a>
			<a class="sns-btn-float" onclick="ga('send','event','visit_sns','click','twitter');" target="_blank"
				id="tw_float" href="https://twitter.com/UranaiRankingJp"><img src="/user/img_re/sns-logo-X_round.png" alt="Xへ">
			</a>
			<a class="sns-btn-float" onclick="ga('send','event','visit_sns','click','instagram');" target="_blank"
				id="tw_float" href="https://www.instagram.com/uranairanking/"><img src="/user/img_re/sns-logo-instagram-round.png" alt="instagramへ">
			</a>
			<a class="sns-btn-float" onclick="ga('send','event','visit_sns','click','line');" target="_blank"
				id="tw_float" href="https://liff.line.me/1645278921-kWRPP32q/?accountId=079vxbrx"><img src="/user/img_re/sns-logo-line-round.png" alt="lineへ">
			</a>
		</div>
		</div>
	</div>

	{* 広告プレビューボタン *}
	{if is_local()}
		<div id='ad-preview-btn' class='pos-fixed ad-preview' style='opacity:0.8'>
			<span class='p-hand' onclick="remove_ad_debugger();">X</span>
			<button class='btn btn-default'
				onclick="$('#ad-preview').submit(); return false;">広告{if $ad_demo == 1}off{else}on{/if}</button>
		</div>
		<form method='post' id='ad-preview'>
			{if $ad_demo == 1}
				<input type='hidden' name='ad-demo' value=0>
			{else}
				<input type='hidden' name='ad-demo' value=1>
			{/if}
		</form>
	{/if}

	<div class="modal fade" id="topicMenu">
		<div class="modal-dialog">
			<div class="modal-content">
				<div class="modal-body">
					<ul>
						{if $data.link_basic.defolt !="404"}
							<li class="text-center defolt-link">
								<a href="{sitelink mode="{$data.link_basic.defolt}" }"><i class="fa fa-star"
										aria-hidden="true"></i><span class="icon-title">総合運</span></a>
							</li>
						{/if}
						{if $data.link_basic.love !="404"}
							<li class="text-center love-link">
								<a href="{sitelink mode="{$data.link_basic.love}" }"><i class="fa fa-heart"
										aria-hidden="true"></i><span class="icon-title">恋愛運</span></a>
							</li>
						{/if}
						{if $data.link_basic.work !="404"}
							<li class="text-center work-link">
								<a href="{sitelink mode="{$data.link_basic.work}" }"><i class="fa fa-pencil"
										aria-hidden="true"></i><span class="icon-title">仕事運</span></a>
							</li>
						{/if}
						{if $data.link_basic.money !="404"}
							<li class="text-center money-link">
								<a href="{sitelink mode="{$data.link_basic.money}" }"><i class="fa fa-jpy"
										aria-hidden="true"></i><span class="icon-title">金運</span></a>
							</li>
						{/if}
						<li class="text-center money-link">
							<div data-dismiss="modal" class="p-hand close-bottn">閉じる</div>
						</li>
					</ul>
				</div>
			</div>
		</div>
	</div>

	{if !$config.isApp || $config.appNeedsBottomMenu}
		<div class="navbar ft-menu2 visible-xs">
			<div class="container">
				{block name="ft-topic-menu"}
					<ul class="nav navbar-nav">
						{if !$notfoundpage}
							<li class="text-center p-hand" data-toggle="modal" data-target="#sampleModal"><i class="fa fa-diamond"
									aria-hidden="true"></i><span class="icon-title">各星座</span></li>
						{/if}

						<li class="text-center p-hand" data-toggle="modal" data-target="#topicMenu">
							<i class="fa fa-smile-o" aria-hidden="true"></i>
							<span class="icon-title">他の運勢</span>
						</li>
						<li class="text-center">
							<a href="{sitelink mode="{$data.link_basic.ranking_past}" }"><i class="fa fa-moon-o"
									aria-hidden="true"></i><span class="icon-title">月間・年間</span></a>
						</li>
						<li class="text-center">
							<a href="{sitelink mode="{$data.link_basic.digest}" }"><i class="fa fa-trophy"
									aria-hidden="true"></i><span
									class="icon-title">{if $smarty.now|date_format:'%Y' == '2023'}今年{else}来年{/if}の運勢</span></a>
						</li>

						{* イベント用モーダル画面を開くボタン *}
						{if $event_data}
							<li id="cat-btn">
								<div class="modal-btn-sp modal-start send-analytics">猫の日</div>
							</li>
						{/if}
					</ul>

					<ul class="nav navbar-nav navbar-right social">
						<!--<li class="text-center"><a href=""><i class="fa fa-cog" aria-hidden="true"></i><span class="icon-title">MENU</span></a></li>-->
						{insert loginbtnSP}
					</ul>
				{/block}
			</div>
		</div>
	{/if}

	{*
	<ul class="debug">
		<li><a href="/app-link/ranking-past">app-link/ranking-past</a></li>
		<li><a href="/app-link/default">app-link/default</a></li>
		<li><a href="/app-link/love">app-link/love</a></li>
		<li><a href="/app-link/work">app-link/work</a></li>
	</ul>

	シモン 2018-10-05
	iOSアプリでリンク先動作確認用
	<ul class="debug">
		<li><a href="/test">Ad-Tests</a></li>
	</ul>
	*}
</body>

</html>