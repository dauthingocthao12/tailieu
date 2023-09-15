<?php
/*
    ネットショップ ページ用のテンプレート
*/

get_header();
?>
<main class="net-shop">
	<section>
		<div class="top-header">
			<div class="header-content">
				<div class="top-left rounded-right top-header-net-shop-bg">
					<div class=" top-header-container content-container-md">
						<h2 class="top-header-title"><strong>ネットショップ </strong></h2>
					</div>
				</div>
				<?php get_template_part("shared/parts/header", "time-information") ?>
			</div>
		</div>
	</section>

	<!-- <section class="section-lb-pink">
		<div class="content-container-md section-padding">
			<h2>メッセージ</h2>
			<div class="sep-25"></div>
			<p>楽天市場、ヤフーショッピングで展開するネットショップ<b>「ドッグサポート アエコム」</b>は、正規輸入品や対応の確かな優良商品をスピーディーにお届けします。
			</p>
		</div>
	</section> -->
	<!-- <section class="section-leaf-pink"> -->
	<section>
		<div class="content-container-md section-padding">
			<h2>ネットショップ</h2>
			<div class="sep-25"></div>
			<div class="col-2">
				<div class="col-image">
				<a href="https://www.rakuten.co.jp/wan/" target="_blank">
					<img src="<?= get_template_directory_uri() ?>/shared/images/net-shop/rakuten-net-shop.png" alt="楽天ネットショップ">
				</a>
				</div>
				<div class="col-text">
					<a href="https://www.rakuten.co.jp/wan/" target="_blank">
						<img src="<?= get_template_directory_uri() ?>/shared/images/net-shop/rakuten-logo.png" alt="楽天ネットショップ">
						<span class="text-increase-space">ドッグサポートアイコム楽天市場店</span>
					</a>
					<div class="read-more">
						<a href="https://www.rakuten.co.jp/wan/" target="_blank">
						<!-- <img src="<?= get_template_directory_uri() ?>/shared/images/net-shop/footprint.png" alt="楽天ネットショップ" class="footprint"> -->
						 詳細はこちら<span class="dli-arrow-right"></span></a>
					</div>
				</div>
			</div>
			<div class="sep-15"></div>
			<div class="col-2">
				<div class="col-image">
					<a href="https://store.shopping.yahoo.co.jp/aecom/" target="_blank">
						<img src="<?= get_template_directory_uri() ?>/shared/images/net-shop/yahoo-net-shop.jpg" alt="Yahooネットショップ">
					</a>
				</div>
				<div class="col-text">
					<a href="https://store.shopping.yahoo.co.jp/aecom/" target="_blank">
						<img src="<?= get_template_directory_uri() ?>/shared/images/net-shop/yahoo-logo.png" alt="Yahooネットショップ">
						<span class="text-increase-space">ドッグサポートアイコムYahooショッピング店</span>
					</a>
					<div class="read-more">
						<a href="https://store.shopping.yahoo.co.jp/aecom/" target="_blank">
							<!-- <img src="<?= get_template_directory_uri() ?>/shared/images/net-shop/footprint.png" alt="Yahooネットショップ" class="footprint"> -->
							 詳細はこちら<span class="dli-arrow-right"></span>
						</a>
					</div>
				</div>
			</div>
		</div>
	</section>

	<?= get_template_part("shared/parts/common", "banners") ?>
</main>

<?php
get_footer();
