{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}
<!--center><a href="../libadmin/edit.php?id={$datum.site_id}">[編集]</a></center-->
{if $data.db}

{if !$data.check}
{* 登録確認以外の場合は *}
<div class="text-right">
	{makelink mode="site" action="listing" value="一覧へ戻る" class="btn btn-warning"}
	{makelink mode="site" action="input" id="{$data.db.site_id}" value="編集" class="btn btn-primary"}
</div>
{/if}

<br>

<table class="table table-bordered">
	<tr>
		<th style="width:20%">&nbsp;</th>
		<th style="width:40%">&nbsp;</th>
		<th style="width:40%">&nbsp;</th>
	</tr>
	<tr>
		<th>ID | 親サイトID</th>
		<td>{$data.db.site_id}</td>
		<td>{$data.db.parent_id}</td>
	</tr>
	<tr>
		<th>サイト名</th>
		<td>{$data.db.site_name}</td>
		<td>{$data.db.site_furigana}</td>
	</tr>
	<tr>
		<th>サイト説明文</th>
		<td colspan="2">
			<div class="text-right">
				{if $data.db.site_detail_visible==1}<i class="fa fa-eye"></i>{else}<i class="fa fa-eye-slash"></i>{/if}
				{makelink mode="site" action="site_desc" id="{$data.db.site_id}" value="&#xf040; 編集" class="btn-fa btn btn-xs btn-primary"}
			</div>
			<p class="site-description">
				<b>説明</b><br>
				{$data.db.site_description}
			</p>
			<p class="site-presentation">
				<b>本文</b><br>
				{$data.db.site_presentation}
			</p>
		</td>
	</tr>
	<tr><th>取得時刻</th><td colspan="2">{$data.db.site_get_time}</td></tr>
	<tr>
		<th class="nowrap">取得曜日</th>
		<td colspan="2">
			{if $data.db.site_get_week0}日&nbsp;{/if}
			{if $data.db.site_get_week1}月&nbsp;{/if}
			{if $data.db.site_get_week2}火&nbsp;{/if}
			{if $data.db.site_get_week3}水&nbsp;{/if}
			{if $data.db.site_get_week4}木&nbsp;{/if}
			{if $data.db.site_get_week5}金&nbsp;{/if}
			{if $data.db.site_get_week6}土{/if}
		</td>
	</tr>
	<tr><th>状態</th><td colspan="2">{if $data.db.is_execute == "1"}有効{else}無効{/if}</td></tr>
	<tr><th>過去の日数</th><td colspan="2">{$data.db.past_days}</td></tr>
	<tr><th>未来の日数</th><td colspan="2">{$data.db.future_days}</td></tr>
	<tr><th>前日更新</th><td>{if $data.db.future_flag == "1"}有効{else}無効{/if}</td><td>更新後のURL:{$data.db.updated_url}</td></tr>
	<tr><th>情報更新時刻</th><td colspan="2">{$data.db.limit_time}</td></tr>
	<tr>
		<th>データ取得URL</th>
		<td colspan="2">
			{if $data.db.get_type == "1"}全体URL{/if}
			{if $data.db.get_type == "2"}星座URL{/if}
			{if $data.db.get_type == "3"}その他URL{/if}
		</td>
	</tr>
	<tr><th>その他URL</th><td colspan="2">{$data.db.etc_url}</td></tr>
	<tr><th>運勢別の取得</th><td colspan="2">{if $data.db.site_topic !== "0" }有効{else}無効{/if}</td></tr>
	<tr>
		<th>運勢別データ取得URL</th>
		<td colspan="2">
			{if $data.db.topic_get_type == "1"}全体URL{/if}
			{if $data.db.topic_get_type == "2"}星座URL{/if}
			{if $data.db.topic_get_type == "3"}その他URL{/if}
			{if $data.db.topic_get_type == "4"}運勢ごと{/if}
		</td>
	</tr>

	<tr><th colspan="3" class="text-center">取得用URL</th></tr>
	<tr>
		<th>&nbsp;</th>
		<th>PC</th>
		<th>スマートフォン</th>
	</tr>
	<tr><th>リンクURL</th>
		<td>{$data.db.link_url}</td>
		<td>{$data.db.sp_link_url}</td>
	</tr>
	<tr><th>全体URL</th>
		<td>{$data.db.url}</td>
		<td>{$data.db.sp_url}</td></tr>
	<tr><th>みずがめ座URL</th>
		<td>{$data.db.star1_url}</td>
		<td>{$data.db.sp_star1_url}</td></tr>
	<tr><th>うお座URL</th>
		<td>{$data.db.star2_url}</td>
		<td>{$data.db.sp_star2_url}</td></tr>
	<tr><th>おひつじ座URL</th>
		<td>{$data.db.star3_url}</td>
		<td>{$data.db.sp_star3_url}</td></tr>
	<tr><th>おうし座URL</th>
		<td>{$data.db.star4_url}</td>
		<td>{$data.db.sp_star4_url}</td></tr>
	<tr><th>ふたご座URL</th>
		<td>{$data.db.star5_url}</td>
		<td>{$data.db.sp_star5_url}</td></tr>
	<tr><th>かに座URL</th>
		<td>{$data.db.star6_url}</td>
		<td>{$data.db.sp_star6_url}</td></tr>
	<tr><th>しし座URL</th>
		<td>{$data.db.star7_url}</td>
		<td>{$data.db.sp_star7_url}</td></tr>
	<tr><th>おとめ座URL</th>
		<td>{$data.db.star8_url}</td>
		<td>{$data.db.sp_star8_url}</td></tr>
	<tr><th>てんびん座URL</th>
		<td>{$data.db.star9_url}</td>
		<td>{$data.db.sp_star9_url}</td></tr>
	<tr><th>さそり座URL</th>
		<td>{$data.db.star10_url}</td>
		<td>{$data.db.sp_star10_url}</td></tr>
	<tr><th>いて座URL</th>
		<td>{$data.db.star11_url}</td>
		<td>{$data.db.sp_star11_url}</td></tr>
	<tr><th>やぎ座URL</th>
		<td>{$data.db.star12_url}</td>
		<td>{$data.db.sp_star12_url}</td></tr>
	<tr><th colspan="3" class="text-center">リンク用URL（総合運）<small>必要な場合のみ入力</small></th></tr>
	<tr>
		<th>データ取得タイプ</th>
		<td colspan="2">
			{if $data.db.link_get_type == "1"}全体URL{/if}
			{if $data.db.link_get_type == "2"}星座URL{/if}
			{if $data.db.link_get_type == "3"}その他URL{/if}
			{if $data.db.link_get_type == "4"}運勢ごと{/if}
		</td>
	</tr>
	<tr><th>&nbsp;</th><th>PC</th><th>スマートフォン</th></tr>
	<tr><th>全体URL</th>
		<td>{$data.db.all_link_url}</td>
		<td>{$data.db.all_sp_link_url}</td></tr>
	<tr><th>みずがめ座URL</th>
		<td>{$data.db.star1_link_url}</td>
		<td>{$data.db.sp_star1_link_url}</td></tr>
	<tr><th>うお座URL</th>
		<td>{$data.db.star2_link_url}</td>
		<td>{$data.db.sp_star2_link_url}</td></tr>
	<tr><th>おひつじ座URL</th>
		<td>{$data.db.star3_link_url}</td>
		<td>{$data.db.sp_star3_link_url}</td></tr>
	<tr><th>おうし座URL</th>
		<td>{$data.db.star4_link_url}</td>
		<td>{$data.db.sp_star4_link_url}</td></tr>
	<tr><th>ふたご座URL</th>
		<td>{$data.db.star5_link_url}</td>
		<td>{$data.db.sp_star5_link_url}</td></tr>
	<tr><th>かに座URL</th>
		<td>{$data.db.star6_link_url}</td>
		<td>{$data.db.sp_star6_link_url}</td></tr>
	<tr><th>しし座URL</th>
		<td>{$data.db.star7_link_url}</td>
		<td>{$data.db.sp_star7_link_url}</td></tr>
	<tr><th>おとめ座URL</th>
		<td>{$data.db.star8_link_url}</td>
		<td>{$data.db.sp_star8_link_url}</td></tr>
	<tr><th>てんびん座URL</th>
		<td>{$data.db.star9_link_url}</td>
		<td>{$data.db.sp_star9_link_url}</td></tr>
	<tr><th>さそり座URL</th>
		<td>{$data.db.star10_link_url}</td>
		<td>{$data.db.sp_star10_link_url}</td></tr>
	<tr><th>いて座URL</th>
		<td>{$data.db.star11_link_url}</td>
		<td>{$data.db.sp_star11_link_url}</td></tr>
	<tr><th>やぎ座URL</th>
		<td>{$data.db.star12_link_url}</td>
		<td>{$data.db.sp_star12_link_url}</td></tr>
		<tr><th colspan="3" class="text-center">リンク用URL（恋愛運）<small>必要な場合のみ入力</small></th></tr>
	<tr>
		<th>データ取得タイプ</th>
		<td colspan="2">
			{if $data.db.love_link_get_type == "1"}全体URL{/if}
			{if $data.db.love_link_get_type == "2"}星座URL{/if}
			{if $data.db.love_link_get_type == "3"}その他URL{/if}
			{if $data.db.love_link_get_type == "4"}運勢ごと{/if}
		</td>
	</tr>
	<tr><th>&nbsp;</th><th>PC</th><th>スマートフォン</th></tr>
	<tr><th>全体URL</th>
		<td>{$data.db.link_love_url}</td>
		<td>{$data.db.sp_link_love_url}</td></tr>
	<tr><th>みずがめ座URL</th>
		<td>{$data.db.star1_link_love_url}</td>
		<td>{$data.db.sp_star1_link_love_url}</td></tr>
	<tr><th>うお座URL</th>
		<td>{$data.db.star2_link_love_url}</td>
		<td>{$data.db.sp_star2_link_love_url}</td></tr>
	<tr><th>おひつじ座URL</th>
		<td>{$data.db.star3_link_love_url}</td>
		<td>{$data.db.sp_star3_link_love_url}</td></tr>
	<tr><th>おうし座URL</th>
		<td>{$data.db.star4_link_love_url}</td>
		<td>{$data.db.sp_star4_link_love_url}</td></tr>
	<tr><th>ふたご座URL</th>
		<td>{$data.db.star5_link_love_url}</td>
		<td>{$data.db.sp_star5_link_love_url}</td></tr>
	<tr><th>かに座URL</th>
		<td>{$data.db.star6_link_love_url}</td>
		<td>{$data.db.sp_star6_link_love_url}</td></tr>
	<tr><th>しし座URL</th>
		<td>{$data.db.star7_link_love_url}</td>
		<td>{$data.db.sp_star7_link_love_url}</td></tr>
	<tr><th>おとめ座URL</th>
		<td>{$data.db.star8_link_love_url}</td>
		<td>{$data.db.sp_star8_link_love_url}</td></tr>
	<tr><th>てんびん座URL</th>
		<td>{$data.db.star9_link_love_url}</td>
		<td>{$data.db.sp_star9_link_love_url}</td></tr>
	<tr><th>さそり座URL</th>
		<td>{$data.db.star10_link_love_url}</td>
		<td>{$data.db.sp_star10_link_love_url}</td></tr>
	<tr><th>いて座URL</th>
		<td>{$data.db.star11_link_love_url}</td>
		<td>{$data.db.sp_star11_link_love_url}</td></tr>
	<tr><th>やぎ座URL</th>
		<td>{$data.db.star12_link_love_url}</td>
		<td>{$data.db.sp_star12_link_love_url}</td></tr>


	<tr><th>備考</th>
		<td colspan="2">{$data.db.comment|escape|nl2br}</td>
	</tr>

	{if !$data.check}
	<tr><th>作成日時</th><td colspan="2">{$data.db.date_create}</td></tr>
	<tr><th>更新日時</th><td colspan="2">{$data.db.date_update}</td></tr>
	{/if}

</table>

{if $data.check}
{* 登録確認の場合は *}
<div class="alert alert-info text-center">
	<p>
	上記の情報を保存しますか？
	</p>
	<br>
	{makelink mode="site" action="update" value="保存する" class="btn btn-primary"}
</div>
<hr>
<div>
	{makelink mode="site" action="input" value="編集" class="btn btn-warning"}
</div>
{/if}

{else}
<strong>データは存在しません</strong>
{/if}
{/block}
