{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}
{* TODO: SITE-EDIT *}

<style>
#star1_url,
#star2_url,
#star3_url,
#star4_url,
#star5_url,
#star6_url,
#star7_url,
#star8_url,
#star9_url,
#star10_url,
#star11_url,
#star12_url
{
	display: block;
	visibility: visible;
}
#comment {
	width:100%;
}
</style>

{if $data.check.error}
<div class="alert alert-danger">
	{$data.check.error}
</div>
{/if}

<form name="f" action="" method="POST">
	<table class="table table-bordered">

		<tr>
			<th width="20%">&nbsp;</th>
			<th width="40%">&nbsp;</th>
			<th width="40%">&nbsp;</th>
		</tr>
		<tr>
			<th class="nowrap">ID | 親サイトID</th>
			<td>{$data.db.site_id}</td>
			<td>
				<input type="text" name="parent_id" value="{$data.db.parent_id}" class="form-control" placeholder="親サイトID">
				{if $data.check.field_errors.parent_id}
				<div class="alert alert-danger">{$data.check.field_errors.parent_id}</div>
				{/if}
			</td>
		</tr>
		<tr><th class="nowrap">サイト名</th>
			<td>
				<input type="text" id="site_name" name="site_name" value="{$data.db.site_name}" class="form-control">
				{if $data.check.field_errors.site_name}
				<div class="alert alert-danger">{$data.check.field_errors.site_name}</div>
				{/if}
			</td>
			<td>
				<input type="text" 
					id="site_furigana"
					name="site_furigana"
					value="{$data.db.site_furigana}" 
					class="form-control"
					placeholder="フリガナ">
				{if $data.check.field_errors.site_furigana}
				<div class="alert alert-danger">{$data.check.field_errors.site_furigana}</div>
				{/if}
			</td>
		</tr>

		<tr>
			<th class="nowrap">取得時刻</th>
			<td>
				<input type="text" id="site_get_time" name="site_get_time" value="{$data.db.site_get_time}" class="form-control">
				{if $data.check.field_errors.site_get_time}
				<div class="alert alert-danger">{$data.check.field_errors.site_get_time}</div>
				{/if}
			</td>
			<td>
				<div class="alert alert-info">
					<b>例：</b> 10:25:00
				</div>
			</td>
		</tr>

		<tr>
			<th class="nowrap">取得曜日</th>
			<td colspan="2">
				<label class="radio-label">
					<input type="checkbox" id="site_get_week0" name="site_get_week0" value="1" {if $data.db.site_get_week0 == "1"}checked{/if} />&nbsp;日 
				</label>
				<label class="radio-label">
					<input type="checkbox" id="site_get_week1" name="site_get_week1" value="1" {if $data.db.site_get_week1 == "1"}checked{/if} />&nbsp;月 
				</label>
				<label class="radio-label">
					<input type="checkbox" id="site_get_week2" name="site_get_week2" value="1" {if $data.db.site_get_week2 == "1"}checked{/if} />&nbsp;火 
				</label>
				<label class="radio-label">
					<input type="checkbox" id="site_get_week3" name="site_get_week3" value="1" {if $data.db.site_get_week3 == "1"}checked{/if} />&nbsp;水 
				</label>
				<label class="radio-label">
					<input type="checkbox" id="site_get_week4" name="site_get_week4" value="1" {if $data.db.site_get_week4 == "1"}checked{/if} />&nbsp;木 
				</label>
				<label class="radio-label">
					<input type="checkbox" id="site_get_week5" name="site_get_week5" value="1" {if $data.db.site_get_week5 == "1"}checked{/if} />&nbsp;金 
				</label>
				<label class="radio-label">
					<input type="checkbox" id="site_get_week6" name="site_get_week6" value="1" {if $data.db.site_get_week6 == "1"}checked{/if} />&nbsp;土
				</label>
				{if $data.check.field_errors.site_get_week}
				<div class="alert alert-danger">{$data.check.field_errors.site_get_week}</div>
				{/if}
			</td>
		</tr>

		<tr>
			<th>有効</th>
			<td colspan="2">
				<input type="hidden" name="is_execute" value="0">
				<input type="checkbox" name="is_execute" value="1" {if $data.db.is_execute == "1"}checked{/if}>
			</td>
		</tr>

		<tr>
			<th>過去の日数</th>
			<td>
				<input type="text" id="past_days" name="past_days" value="{$data.db.past_days|default:0D}" class="form-control">
				{if $data.check.field_errors.past_days}
				<div class="alert alert-danger">{$data.check.field_errors.past_days}</div>
				{/if}
			</td>
			<td rowspan="2">
				<div class="alert alert-info">
					<b>例：</b><br>
					3D は ３日<br>
					1M は 一ヶ月 1Yで1年
				</div>
			</td>
		</tr>

		<tr>
			<th>未来の日数</th>
			<td>
				<input type="text" id="future_days" name="future_days" value="{$data.db.future_days|default:0D}" class="form-control">
				{if $data.check.field_errors.future_days}
				<div class="alert alert-danger">{$data.check.field_errors.future_days}</div>
				{/if}
			</td>
		</tr>
		<tr>
			<th>前日更新(更新時間)</th>
			<td>
				<input type="hidden" id="future_flag" name="future_flag" value="0">
				有<input type="checkbox" id="future_flag" name="future_flag" value="1" {if $data.db.future_flag == "1"}checked{/if}>	
			</td>
			<td>
				更新後のURL:<input type="text" id="updated_url" name="updated_url" value="{$data.db.updated_url|default:""}" class="form-control">
			</td>
		</tr>

		<tr>
			<th>情報更新時刻</th>
			<td>
				<input type="text" id="limit_time" name="limit_time" value="{$data.db.limit_time|default:NULL}" class="form-control">
				{if $data.check.field_errors.limit_time}
				<div class="alert alert-danger">{$data.check.field_errors.limit_time}</div>
				{/if}
			</td>
			<td>
				<div class="alert alert-info">
					空の場合は：NULL<br>
					過去の日数が<strong>0D</strong>の場合のみ指定してください。<br>
					この値を指定しておくと、その日のその時間を過ぎたときにサイト名のリンクに「※」が付きます。
				</div>
			</td>
		</tr>

		<tr>
			<th>データ取得URL</th>
			<td colspan="2">
				<label class="radio-label">
					<input type="radio" name="get_type" value="1" {if $data.db.get_type == "1"}checked{/if}>全体URL
				</label>
				<label class="radio-label">
					<input type="radio" name="get_type" value="2" {if $data.db.get_type == "2"}checked{/if}>星座ごと
				</label>
				<label class="radio-label">
					<input type="radio" name="get_type" value="3" {if $data.db.get_type == "3"}checked{/if}>その他
				</label>
				{if $data.check.field_errors.get_type}
				<div class="alert alert-danger">{$data.check.field_errors.get_type}</div>
				{/if}
			</td>
		</tr>

		<tr>
			<th>その他URL</th>
			<td colspan="2">
				<input type="text" id="etc_url" name="etc_url" value="{$data.db.etc_url}" class="form-control">
				{if $data.check.field_errors.etc_url}
				<div class="alert alert-danger">{$data.check.field_errors.etc_url}</div>
				{/if}
			</td>
		</tr>
		<tr>
			<th>運勢取得　有効</th>
			<td colspan="2">
				<input type="hidden" name="site_topic" value="0">
				<input type="checkbox" name="site_topic" value="1" {if $data.db.site_topic == true}checked{/if}>
			</td>
		</tr>
		<tr>
			<th>運勢データ取得URL</th>
			<td colspan="2">
				<label class="radio-label">
					<input type="radio" name="topic_get_type" value="1" {if $data.db.topic_get_type == "1"}checked{/if}>全体URL
				</label>
				<label class="radio-label">
					<input type="radio" name="topic_get_type" value="2" {if $data.db.topic_get_type == "2"}checked{/if}>星座ごと
				</label>
				<label class="radio-label">
					<input type="radio" name="topic_get_type" value="3" {if $data.db.topic_get_type == "3"}checked{/if}>その他
				</label>
				<label class="radio-label">
					<input type="radio" name="topic_get_type" value="4" {if $data.db.topic_get_type == "4"}checked{/if}>運勢ごと
				</label>
				{if $data.check.field_errors.topic_get_type}
				<div class="alert alert-danger">{$data.check.field_errors.topic_get_type}</div>
				{/if}
			</td>
		</tr>
		<tr><th colspan="3" class="text-center">取得用URL</th></tr>
		<tr>
			<td colspan="3">
				<div class="alert alert-info">
					URLに日付を持つ場合の入力ルール<br>
					(md):月日各2桁 例) 0808 (8月8日)　　(ymd):年月日各2桁 例) 170808 (2017年8月8日)　　(Y):年4桁 例) 2017 (年)<br>
					(M):月1桁 例) 8 (月)　　(m):月2桁 例) 08 (月)　　(d):日1桁 例) 8 (日)　　(dd):日2桁 例) 08 (日)<br>
					リンクURLにのみ使用可　　(yesterday):昨日の日付2桁 例) 07 (08日)
				</div>
			</td>
		</tr>
		<tr>
			<th>&nbsp;</th><th>PC</th><th>スマートフォン</th>
		</tr>
		<tr>
			<th>リンクURL</th>
			<td>
				<input type="text" id="link_url" name="link_url" value="{$data.db.link_url}" class="form-control">
				{if $data.check.field_errors.link_url}
				<div class="alert alert-danger">{$data.check.field_errors.link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_link_url" name="sp_link_url" value="{$data.db.sp_link_url}" class="form-control"></td>
		</tr>
		
		<tr>
			<th>
				全体URL
			</th>
			<td>
				<input type="text" id="url" name="url" value="{$data.db.url}" class="form-control">
				{if $data.check.field_errors.url}
				<div class="alert alert-danger">{$data.check.field_errors.url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_url" name="sp_url" value="{$data.db.sp_url}" class="form-control"></td>
		</tr>

		<tr><th>みずがめ座URL</th>
			<td>
				<input type="text" id="star1_url" name="star1_url" value="{$data.db.star1_url}" class="form-control">
				{if $data.check.field_errors.star1_url}
				<div class="alert alert-danger">{$data.check.field_errors.star1_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star1_url" name="sp_star1_url" value="{$data.db.sp_star1_url}" class="form-control"></td>
		</tr>

		<tr><th>うお座URL</th>
			<td>
				<input type="text" id="star2_url" name="star2_url" value="{$data.db.star2_url}" class="form-control">
				{if $data.check.field_errors.star2_url}
				<div class="alert alert-danger">{$data.check.field_errors.star2_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star2_url" name="sp_star2_url" value="{$data.db.sp_star2_url}" class="form-control"></td>
		</tr>

		<tr><th>おひつじ座URL</th>
			<td>
				<input type="text" id="star3_url" name="star3_url" value="{$data.db.star3_url}" class="form-control">
				{if $data.check.field_errors.star3_url}
				<div class="alert alert-danger">{$data.check.field_errors.star3_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star3_url" name="sp_star3_url" value="{$data.db.sp_star3_url}" class="form-control"></td>
		</tr>

		<tr><th>おうし座URL</th>
			<td>
				<input type="text" id="star4_url" name="star4_url" value="{$data.db.star4_url}" class="form-control">
				{if $data.check.field_errors.star4_url}
				<div class="alert alert-danger">{$data.check.field_errors.star4_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star4_url" name="sp_star4_url" value="{$data.db.sp_star4_url}" class="form-control"></td>
		</tr>

		<tr><th>ふたご座URL</th>
			<td>
				<input type="text" id="star5_url" name="star5_url" value="{$data.db.star5_url}" class="form-control">
				{if $data.check.field_errors.star5_url}
				<div class="alert alert-danger">{$data.check.field_errors.star5_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star5_url" name="sp_star5_url" value="{$data.db.sp_star5_url}" class="form-control"></td>
		</tr>

		<tr><th>かに座URL</th>
			<td>
				<input type="text" id="star6_url" name="star6_url" value="{$data.db.star6_url}" class="form-control">
				{if $data.check.field_errors.star6_url}
				<div class="alert alert-danger">{$data.check.field_errors.star6_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star6_url" name="sp_star6_url" value="{$data.db.sp_star6_url}" class="form-control"></td>
		</tr>
		
		<tr><th>しし座URL</th>
			<td>
				<input type="text" id="star7_url" name="star7_url" value="{$data.db.star7_url}" class="form-control">
				{if $data.check.field_errors.star7_url}
				<div class="alert alert-danger">{$data.check.field_errors.star7_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star7_url" name="sp_star7_url" value="{$data.db.sp_star7_url}" class="form-control"></td>
		</tr>

		<tr><th>おとめ座URL</th>
			<td>
				<input type="text" id="star8_url" name="star8_url" value="{$data.db.star8_url}" class="form-control">
				{if $data.check.field_errors.star8_url}
				<div class="alert alert-danger">{$data.check.field_errors.star8_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star8_url" name="sp_star8_url" value="{$data.db.sp_star8_url}" class="form-control"></td>
		</tr>

		<tr><th>てんびん座URL</th>
			<td>
				<input type="text" id="star9_url" name="star9_url" value="{$data.db.star9_url}" class="form-control">
				{if $data.check.field_errors.star9_url}
				<div class="alert alert-danger">{$data.check.field_errors.star9_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star9_url" name="sp_star9_url" value="{$data.db.sp_star9_url}" class="form-control"></td>
		</tr>

		<tr><th>さそり座URL</th>
			<td>
				<input type="text" id="star10_url" name="star10_url" value="{$data.db.star10_url}" class="form-control">
				{if $data.check.field_errors.star10_url}
				<div class="alert alert-danger">{$data.check.field_errors.star10_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star10_url" name="sp_star10_url" value="{$data.db.sp_star10_url}" class="form-control"></td>
		</tr>

		<tr><th>いて座URL</th>
			<td>
				<input type="text" id="star11_url" name="star11_url" value="{$data.db.star11_url}" class="form-control">
				{if $data.check.field_errors.star11_url}
				<div class="alert alert-danger">{$data.check.field_errors.star11_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star11_url" name="sp_star11_url" value="{$data.db.sp_star11_url}" class="form-control"></td>
		</tr>

		<tr><th>やぎ座URL</th>
			<td>
				<input type="text" id="star23_url" name="star12_url" value="{$data.db.star12_url}" class="form-control">
				{if $data.check.field_errors.star12_url}
				<div class="alert alert-danger">{$data.check.field_errors.star12_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star12_url" name="sp_star12_url" value="{$data.db.sp_star12_url}" class="form-control"></td>
		</tr>
		
		<!-------------------- リンク用（総合運） -------------------->
		<tr><th colspan="3" class="text-center">リンク用URL（総合運）<small>必要な場合のみ入力</small></th></tr>
		<tr>
			<th>運勢データ取得URL</th>
			<td colspan="2">
				<label class="radio-label">
					<input type="radio" name="link_get_type" value="1" {if $data.db.link_get_type == "1"}checked{/if}>全体URL
				</label>
				<label class="radio-label">
					<input type="radio" name="link_get_type" value="2" {if $data.db.link_get_type == "2"}checked{/if}>星座ごと
				</label>
				<label class="radio-label">
					<input type="radio" name="link_get_type" value="3" {if $data.db.link_get_type == "3"}checked{/if}>その他
				</label>
				<label class="radio-label">
					<input type="radio" name="link_get_type" value="4" {if $data.db.link_get_type == "4"}checked{/if}>運勢ごと
				</label>
				{if $data.check.field_errors.topic_get_type}
				<div class="alert alert-danger">{$data.check.field_errors.topic_get_type}</div>
				{/if}
			</td>
		</tr>
		
		<tr>
			<th>&nbsp;</th><th>PC</th><th>スマートフォン</th>
		</tr>

		<tr>
			<th>
				全体URL
			</th>
			<td>
				<input type="text" id="all_link_url" name="all_link_url" value="{$data.db.all_link_url}" class="form-control">
			</td>
			<td><input type="text" id="all_sp_link_url" name="all_sp_link_url" value="{$data.db.all_sp_link_url}" class="form-control"></td>
		</tr>
		<tr><th>みずがめ座リンクURL</th>
			<td>
				<input type="text" id="star1_link_url" name="star1_link_url" value="{$data.db.star1_link_url}" class="form-control">
				{if $data.check.field_errors.star1_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star1_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star1_link_url" name="sp_star1_link_url" value="{$data.db.sp_star1_link_url}" class="form-control"></td>
		</tr>

		<tr><th>うお座リンクURL</th>
			<td>
				<input type="text" id="star2_link_url" name="star2_link_url" value="{$data.db.star2_link_url}" class="form-control">
				{if $data.check.field_errors.star2_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star2_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star2_link_url" name="sp_star2_link_url" value="{$data.db.sp_star2_link_url}" class="form-control"></td>
		</tr>

		<tr><th>おひつじ座リンクURL</th>
			<td>
				<input type="text" id="star3_link_url" name="star3_link_url" value="{$data.db.star3_link_url}" class="form-control">
				{if $data.check.field_errors.star3_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star3_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star3_link_url" name="sp_star3_link_url" value="{$data.db.sp_star3_link_url}" class="form-control"></td>
		</tr>

		<tr><th>おうし座リンクURL</th>
			<td>
				<input type="text" id="star4_link_url" name="star4_link_url" value="{$data.db.star4_link_url}" class="form-control">
				{if $data.check.field_errors.star4_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star4_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star4_link_url" name="sp_star4_link_url" value="{$data.db.sp_star4_link_url}" class="form-control"></td>
		</tr>

		<tr><th>ふたご座リンクURL</th>
			<td>
				<input type="text" id="star5_link_url" name="star5_link_url" value="{$data.db.star5_link_url}" class="form-control">
				{if $data.check.field_errors.star5_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star5_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star5_link_url" name="sp_star5_link_url" value="{$data.db.sp_star5_link_url}" class="form-control"></td>
		</tr>

		<tr><th>かに座リンクURL</th>
			<td>
				<input type="text" id="star6_link_url" name="star6_link_url" value="{$data.db.star6_link_url}" class="form-control">
				{if $data.check.field_errors.star6_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star6_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star6_link_url" name="sp_star6_link_url" value="{$data.db.sp_star6_link_url}" class="form-control"></td>
		</tr>
		
		<tr><th>しし座リンクURL</th>
			<td>
				<input type="text" id="star7_link_url" name="star7_link_url" value="{$data.db.star7_link_url}" class="form-control">
				{if $data.check.field_errors.star7_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star7_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star7_link_url" name="sp_star7_link_url" value="{$data.db.sp_star7_link_url}" class="form-control"></td>
		</tr>

		<tr><th>おとめ座リンクURL</th>
			<td>
				<input type="text" id="star8_link_url" name="star8_link_url" value="{$data.db.star8_link_url}" class="form-control">
				{if $data.check.field_errors.star8_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star8_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star8_link_url" name="sp_star8_link_url" value="{$data.db.sp_star8_link_url}" class="form-control"></td>
		</tr>

		<tr><th>てんびん座リンクURL</th>
			<td>
				<input type="text" id="star9_link_url" name="star9_link_url" value="{$data.db.star9_link_url}" class="form-control">
				{if $data.check.field_errors.star9_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star9_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star9_link_url" name="sp_star9_link_url" value="{$data.db.sp_star9_link_url}" class="form-control"></td>
		</tr>

		<tr><th>さそり座リンクURL</th>
			<td>
				<input type="text" id="star10_link_url" name="star10_link_url" value="{$data.db.star10_link_url}" class="form-control">
				{if $data.check.field_errors.star10_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star10_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star10_link_url" name="sp_star10_link_url" value="{$data.db.sp_star10_link_url}" class="form-control"></td>
		</tr>

		<tr><th>いて座リンクURL</th>
			<td>
				<input type="text" id="star11_link_url" name="star11_link_url" value="{$data.db.star11_link_url}" class="form-control">
				{if $data.check.field_errors.star11_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star11_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star11_link_url" name="sp_star11_link_url" value="{$data.db.sp_star11_link_url}" class="form-control"></td>
		</tr>

		<tr><th>やぎ座リンクURL</th>
			<td>
				<input type="text" id="star23_link_url" name="star12_link_url" value="{$data.db.star12_link_url}" class="form-control">
				{if $data.check.field_errors.star12_link_url}
				<div class="alert alert-danger">{$data.check.field_errors.star12_link_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star12_link_url" name="sp_star12_link_url" value="{$data.db.sp_star12_link_url}" class="form-control"></td>
		</tr>

		<!-------------------- リンク用（恋愛運） -------------------->
		<tr><th colspan="3" class="text-center">リンク用URL（恋愛運）<small>必要な場合のみ入力</small></th></tr>
		
		<tr>
			<th>運勢データ取得URL</th>
			<td colspan="2">
				<label class="radio-label">
					<input type="radio" name="love_link_get_type" value="1" {if $data.db.love_link_get_type == "1"}checked{/if}>全体URL
				</label>
				<label class="radio-label">
					<input type="radio" name="love_link_get_type" value="2" {if $data.db.love_link_get_type == "2"}checked{/if}>星座ごと
				</label>
				<label class="radio-label">
					<input type="radio" name="love_link_get_type" value="3" {if $data.db.love_link_get_type == "3"}checked{/if}>その他
				</label>
				<label class="radio-label">
					<input type="radio" name="love_link_get_type" value="4" {if $data.db.love_link_get_type == "4"}checked{/if}>運勢ごと
				</label>
				{if $data.check.field_errors.topic_get_type}
				<div class="alert alert-danger">{$data.check.field_errors.topic_get_type}</div>
				{/if}
			</td>
		</tr>
		<tr>
			<th>&nbsp;</th><th>PC</th><th>スマートフォン</th>
		</tr>

		<tr>
			<th>
				全体URL
			</th>
			<td>
				<input type="text" id="link_love_url" name="link_love_url" value="{$data.db.link_love_url}" class="form-control">
			</td>
			<td><input type="text" id="sp_link_love_url" name="sp_link_love_url" value="{$data.db.sp_link_love_url}" class="form-control"></td>
		</tr>
		<tr><th>みずがめ座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star1_link_love_url" name="star1_link_love_url" value="{$data.db.star1_link_love_url}" class="form-control">
				{if $data.check.field_errors.star1_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star1_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star1_link_love_url" name="sp_star1_link_love_url" value="{$data.db.sp_star1_link_love_url}" class="form-control"></td>
		</tr>

		<tr><th>うお座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star2_link_love_url" name="star2_link_love_url" value="{$data.db.star2_link_love_url}" class="form-control">
				{if $data.check.field_errors.star2_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star2_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star2_link_love_url" name="sp_star2_link_love_url" value="{$data.db.sp_star2_link_love_url}" class="form-control"></td>
		</tr>

		<tr><th>おひつじ座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star3_link_love_url" name="star3_link_love_url" value="{$data.db.star3_link_love_url}" class="form-control">
				{if $data.check.field_errors.star3_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star3_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star3_link_love_url" name="sp_star3_link_love_url" value="{$data.db.sp_star3_link_love_url}" class="form-control"></td>
		</tr>

		<tr><th>おうし座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star4_link_love_url" name="star4_link_love_url" value="{$data.db.star4_link_love_url}" class="form-control">
				{if $data.check.field_errors.star4_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star4_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star4_link_love_url" name="sp_star4_link_love_url" value="{$data.db.sp_star4_link_love_url}" class="form-control"></td>
		</tr>

		<tr><th>ふたご座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star5_link_love_url" name="star5_link_love_url" value="{$data.db.star5_link_love_url}" class="form-control">
				{if $data.check.field_errors.star5_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star5_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star5_link_love_url" name="sp_star5_link_love_url" value="{$data.db.sp_star5_link_love_url}" class="form-control"></td>
		</tr>

		<tr><th>かに座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star6_link_love_url" name="star6_link_love_url" value="{$data.db.star6_link_love_url}" class="form-control">
				{if $data.check.field_errors.star6_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star6_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star6_link_love_url" name="sp_star6_link_love_url" value="{$data.db.sp_star6_link_love_url}" class="form-control"></td>
		</tr>
		
		<tr><th>しし座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star7_link_love_url" name="star7_link_love_url" value="{$data.db.star7_link_love_url}" class="form-control">
				{if $data.check.field_errors.star7_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star7_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star7_link_love_url" name="sp_star7_link_love_url" value="{$data.db.sp_star7_link_love_url}" class="form-control"></td>
		</tr>

		<tr><th>おとめ座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star8_link_love_url" name="star8_link_love_url" value="{$data.db.star8_link_love_url}" class="form-control">
				{if $data.check.field_errors.star8_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star8_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star8_link_love_url" name="sp_star8_link_love_url" value="{$data.db.sp_star8_link_love_url}" class="form-control"></td>
		</tr>

		<tr><th>てんびん座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star9_link_love_url" name="star9_link_love_url" value="{$data.db.star9_link_love_url}" class="form-control">
				{if $data.check.field_errors.star9_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star9_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star9_link_love_url" name="sp_star9_link_love_url" value="{$data.db.sp_star9_link_love_url}" class="form-control"></td>
		</tr>

		<tr><th>さそり座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star10_link_love_url" name="star10_link_love_url" value="{$data.db.star10_link_love_url}" class="form-control">
				{if $data.check.field_errors.star10_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star10_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star10_link_love_url" name="sp_star10_link_love_url" value="{$data.db.sp_star10_link_love_url}" class="form-control"></td>
		</tr>

		<tr><th>いて座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star11_link_love_url" name="star11_link_love_url" value="{$data.db.star11_link_love_url}" class="form-control">
				{if $data.check.field_errors.star11_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star11_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star11_link_love_url" name="sp_star11_link_love_url" value="{$data.db.sp_star11_link_love_url}" class="form-control"></td>
		</tr>

		<tr><th>やぎ座リンクURL（恋愛）</th>
			<td>
				<input type="text" id="star23_link_love_url" name="star12_link_love_url" value="{$data.db.star12_link_love_url}" class="form-control">
				{if $data.check.field_errors.star12_link_love_url}
				<div class="alert alert-danger">{$data.check.field_errors.star12_link_love_url}</div>
				{/if}
			</td>
			<td><input type="text" id="sp_star12_link_love_url" name="sp_star12_link_love_url" value="{$data.db.sp_star12_link_love_url}" class="form-control"></td>
		</tr>

		<tr><th>備考</th><td colspan="2">
				<textarea rows="5" cols="61" id="comment" name="comment" class="form-control">{$data.db.comment}</textarea></td>
		</tr>
		<!--<tr><th>作成日時</th>-->
		<!--	<td colspan="3">{$data.db.date_create}</td>-->
		<!--</tr>-->
		<!--<tr><th>更新日時</th>-->
		<!--	<td colspan="3">{$data.db.date_update}</td>-->
		<!--</tr>-->
	</table>

	<div class="text-center">
		<input type="hidden" name="mode" value="site">
		<input type="hidden" name="action" value="check">
		<input type="hidden" name="magic" value="update">
		<input type="hidden" name="magic2" value="readonly">
		<input type="hidden" name="site_id" value="{$data.db.site_id}">
		<input type="submit" value="確認" class="btn btn-primary">
	</div>
</form>

<hr>
{makelink mode="site" action="listing" value="一覧へ戻る" class="btn btn-warning"}
{/block}
