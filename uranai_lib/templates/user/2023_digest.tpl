{extends file="main.tpl"}
{block name=seo}
{nocache}
{if $data.date == $date.now}
<title>12星座占い ランキング | 星占い {$seo.date}の{$data.star_name}{if $data.data_type != ''}の{$data.topic_name}{/if}</title>
<meta name="keywords" content="12星座占いランキング,{$data.star_name},星座占い,まとめ,{$seo.topic_or_unsei},ランキング,{$data.date}">
<meta name="description" content="各サイトから取得した12星座占いの{$seo.topic_or_empty}ランキング情報を集計し、{$seo.date}の『{$data.star_name}』の結果をまとめたものを表示しています。ここ１週間の順位変化のグラフも表示しています。閲覧は無料です。ユーザー登録するとメールで毎日結果をお知らせ♪集計サイトも随時更新中!!">
{/if}
{/nocache}
<!--OGP START-->
{include file="ogp.tpl" title="｜今年の運勢"
		des="各サイトから取得した12星座占いの{$seo.topic_or_empty}ランキング情報を集計し、{$seo.date}の『{$data.star_name}』の結果をまとめたものを表示しています。ここ１週間の順位変化のグラフも表示しています。閲覧は無料です。ユーザー登録するとメールで毎日結果をお知らせ♪集計サイトも随時更新中!!"}
<!--OGP END--> 
{/block}

{block name="body"}

		<div class="ranking-details margin-content">
			<div class="title row text-center font-color col-md-9">
					<h2 class="font-color">2023年の占いサイトまとめ</h2>
			</div>
			<div class="title row text-center font-color col-md-3">
			</div>

			{* 広告 *}
			{* <div class="text-center">{insert ad_group id="1"}</div> *}

			<div class="row col-md-9">
				<div class="tecen digest-list-inner ">
				<div class="digest-intro digest-font-main">
					<p>2023年全体の占いサイトをまとめました。<br>この中から、あなたにとって良い結果が見つけられますように…。<br>2023年もハッピーな気分で過ごしましょう♪</p>
				</div>
				<div class="digest-detail">
					{if !empty($digest_list)}
					{foreach $digest_list as $val}
					<div class="text-left">
						{if $val@iteration%4 == 0}

							{* <h4 class="digest-site-name digest-font-main">PR</h4> del kitakaze 2021/12/17  *}
							{if $val@iteration == 8}
							<h4 class="digest-site-name digest-font-main"><a href="{$val.url}" target="_brank">{$val.name}</a></h4>
							<p class="site-me-dtl">{$val.description}</p>
							<h4 class="digest-site-name digest-font-main">PR</h4>
								<div class="site-me-dtl">{insert ad_group id="9"}</div>
							{else}
							<h4 class="digest-site-name digest-font-main"><a href="{$val.url}" target="_brank">{$val.name}</a></h4>
							<p class="site-me-dtl">{$val.description}</p>

							<h4 class="digest-site-name digest-font-main">PR</h4>
								<div class="site-me-dtl">{insert ad_group id="8"}</div>
							{/if}
						{else}
							<h4 class="digest-site-name digest-font-main"><a href="{$val.url}" target="_brank">{$val.name}</a></h4>
							<p class="site-me-dtl">{$val.description}</p>
						{/if}
					</div>
					{/foreach}
					{else}
					<p>表示データを取得できませんでした。</p>
					{/if}
				</div>
				<p class="conclusion">色々なサイトをまとめましたが、サイトによってさまざまな結果だと思います。<br>当たるも八卦当たらぬも八卦！<br>2023年もハッピーに過ごしましょう♪</p>
				</div>
			</div>

			<div class="side ">
				{include file='sidebar.tpl'}
			</div>
		</div>
{/block}
