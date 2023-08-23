			<div class="col-md-3 col-xs-12 sidebar">
				{insert name="lastNews" assign="lastnews"}
				{if $lastnews}
				<div class="spadding">
					<div class="panel panel-info">
						<div class="panel-heading">
							新着情報
						</div>
						<div class="panel-body">
							{$lastnews}
						</div>
					</div>
				</div>
				{/if}

				
				
				

				<div class="tecen spadding">
					<a href="{sitelink mode='ranking-past'}">
					<img loading="lazy"  class="banner hidden-xs hidden-sm width-max" src="/user/img_re/year-month.png" alt="過去の年間・月間ランキングをチェック">
					<img loading="lazy"  id="ranking-past-banner" class="visible-xs visible-sm width-max" src="/user/img_re/year-month-2.png" alt="過去の年間・月間ランキングをチェック">
					</a>
				</div>
			{if $sns_on}
			<div class="tecen spadding" itemscope="" itemtype="http://schema.org/Organization">
				<link itemprop="url" href="https://uranairanking.jp/">
				<div class="clearfix">
					<div class="sns-title" id="sns-links">12星座占いのSNSはこちら!</div>
					<div class="sns-area">
						<a itemprop="sameAs" class="social-btn square_btn" href="https://twitter.com/UranaiRankingJp" target="_blank">
							<img loading="lazy"  alt="X" src="/user/img_re/sns-logo-X.png"><span class="social-btn-text">12星座占いの<span class="word-break">Xへ</span></span>
						</a>
						<a itemprop="sameAs" class="social-btn square_btn" href="https://www.facebook.com/12&#x661f;&#x5ea7;&#x5360;&#x3044;&#x30e9;&#x30f3;&#x30ad;&#x30f3;&#x30b0;-517659478433475" target="_blank">
							<img loading="lazy"  alt="facebook" src="/user/img_re/sns-logo-facebook.png"><span class="social-btn-text">12星座占いの<span class="word-break">Facebookへ</span></span>
						</a>
						<a itemprop="sameAs" class="social-btn square_btn" href="https://www.instagram.com/uranairanking/" target="_blank">
							<img loading="lazy"  alt="instagram" src="/user/img_re/sns-logo-instagram.png"><span class="social-btn-text">12星座占いの<span class="word-break">Instagramへ</span></span>
						</a>
						<a itemprop="sameAs" class="social-btn square_btn" href="https://liff.line.me/1645278921-kWRPP32q/?accountId=079vxbrx" target="_blank">
						<img loading="lazy"  alt="line" src="/user/img_re/sns-logo-line.png"><span class="social-btn-text">12星座占いの<span class="word-break">Lineへ</span></span>
					</a>
					</div>
				</div>
			</div>
			{/if}
			{if !$config.isApp}
				<div class="application clearfix" id="application-area">
				<div class="col-md-12 col-sm-6 clear">
					<a href="https://play.google.com/store/apps/details?id=jp.uranairanking.uranairanking&hl=ja" target="_blank">
					<img loading="lazy"  src="/user/img/google-play-badge.png" class="width-max" alt="Google Play で手に入れよう"></a>

				</div>
				<div class="col-md-12 col-sm-6 text-center app-badge-spacer">
					<div class="ios-badge-wrapper">
						{* <a class="iOS-app" style='background: url("//linkmaker.itunes.apple.com/assets/shared/badges/ja-jp/appstore-lrg.svg") no-repeat 0% 0% / contain; width: 100%; padding-top: 30%; overflow: hidden; display: inline-block;' href="https://apps.apple.com/us/app/12%E6%89%9F%E5%BA%A7%E5%8D%A0%E3%81%84%E3%83%A9%E3%83%B3%E3%82%AD%E3%83%B3%E3%82%B0/id1235436615?itsct=apps_box_badge&amp;itscg=30200"></a> *}
						<a class="ios-app" target="blank" href="https://apps.apple.com/us/app/12%E6%89%9F%E5%BA%A7%E5%8D%A0%E3%81%84%E3%83%A9%E3%83%B3%E3%82%AD%E3%83%B3%E3%82%B0/id1235436615?itsct=apps_box_badge&amp;itscg=30200" ><img src="https://tools.applemediaservices.com/api/badges/download-on-the-app-store/black/ja-jp?size=250x83&amp;releaseDate=1497052800&h=7a9cc76bc22da642370c4574ecf36b35" alt="Download on the App Store" style="border-radius: 13px;"></a>
					</div>
				</div>
				</div>
			{/if}
			
			</div>
