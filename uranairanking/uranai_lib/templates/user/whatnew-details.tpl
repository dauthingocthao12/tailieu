{extends file="main.tpl"}
{block name=seo}
<title>新着情報 12星座占いランキング|{$news_details.news_title|strip_tags}</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp,新着情報">
<meta name="description" content="新着情報({$news_details.news_release_date|seodate})。{$news_details.news_title|strip_tags}">
{/block}

{block name=body}
<div class="container whatnew">
	<div class="title row text-center">
		<h2 class="font-color">新着情報</h2>
		{include "mainline.parts.tpl"}
	</div>
	<div class="col-md-9">
		<hr class="ad_sep">
		{* 2023/05/19 お詫び掲載につきコメントアウト *}
		{* <div class="ad-bg adg-news modal-ad">
			{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
			{insert ad_group id="6"}
			{else}
			{insert ad_group id="5"}
			{/if}
		</div> *}
		{* コメントアウトここまで *}

		<div class="col-xs-12 base-bg contents-space">
			<h2>{$news_details.news_title}</h2>
			<hr>
			<div class="news-date text-right">{$news_details.news_release_date|japanesedate}</div>

			<div class="news-content">
				{$news_details.news_content}
			</div>
			<hr>
			<a href="{sitelink mode="whatnew"}" class="btn btn-primary">一覧へ戻る</a>
		</div>

		<hr class="ad_sep">
		{* 2023/05/19 お詫び掲載につきコメントアウト *}
		{* <div class="ad-bg adg-news modal-ad">
			{if $config.browser_name == "IE" || $config.browser_name == "Edge"}
			{insert ad_group id="6"}
			{else}
			{insert ad_group id="4"}
			{/if}
		</div> *}
		{* コメントアウトここまで *}
	</div>
	{include file='sidebar.tpl'}
</div>
{/block}
