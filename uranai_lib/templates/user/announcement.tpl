{extends file="main.tpl"}
{block name=seo}
<title>イベント開催予告 12星座占いランキング</title>
<meta name="keywords" content="12星座占いランキング,占い,ランキング,イベント予告：星に願いをこめて,七夕">
<meta name="description" content="12星座占いランキングのイベント開催予告です。">
{/block}
{block name=body}
<div class="container announcement">
		<div class="title row tecen">
			<h2 class="font-color">開催予告　星空に願いをこめて</h2>
			{include "mainline.parts.tpl"}
		</div>
		<div class="ad-bg modal-ad">
			<div class="hidden-xs">
				<script src="//i.socdm.com/sdk/js/adg-script-loader.js?id=50207&targetID=adg_50207&displayid=2&adType=TABLET&async=false&tagver=2.0.0"></script>
			</div>
			<div class="visible-xs">
				<script src="//i.socdm.com/sdk/js/adg-script-loader.js?id=50231&targetID=adg_50231&displayid=2&adType=SP&async=false&tagver=2.0.0"></script>
			</div>
		</div>

		<div class="ev_img">
			{if $design_name.remaining}
			<img src="/user/img_re/{$design_name.ev_name}start.jpg" alt="七夕イベント実施中" width="100%">
			{elseif $announcement.remaining}
			<img src="/user/img_re/{$announcement.ev_name}{$announcement.remaining}.jpg" alt="公開まであと{$announcement.remaining}日" width="100%">
			{else} 
			<img src="/user/img_re/{$design_name.ev_end_name}end.jpg" alt="イベントは終了しました。" width="100%">
			{/if}
		</div>
		
		<h5 class="tecen ev_message">7月7日の夜は年に一度織姫と彦星が再開できると言い伝えられている日です。<br>12星座占いランキングでは、七夕イベントとして6/30日より、デザインを変更いたします♪<br>「家で七夕を飾るのは少し大変…」「時間がない…」という方もちょっとした七夕気分が味わえますので、限定デザインをぜひお楽しみください♪
		</h5>
		
		<div class="ad-bg modal-ad">
			<div class="hidden-xs">
				<script src="//i.socdm.com/sdk/js/adg-script-loader.js?id=50207&targetID=adg_50207&displayid=2&adType=TABLET&async=false&tagver=2.0.0"></script>
			</div>
			<div class="visible-xs">
				<script src="//i.socdm.com/sdk/js/adg-script-loader.js?id=50233&targetID=adg_50233&displayid=3&adType=SP&async=false&tagver=2.0.0"></script>

			</div>
		</div>

	</div><!--end base-bg-->
</div>
{/block}