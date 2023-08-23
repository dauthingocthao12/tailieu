{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}
{literal}
<script>
function checkXSS() {
	var frm = document.getElementsByClassName("form-control");
	for(var i=0; i < frm.length; i++){
		if(checkJsTag(frm[i].value)){
			alert("入力欄にスクリプトタグが含まれています。使用しないでください。");
			return false;
		}
	}
}
function checkJsTag(str){
	var reg = /\<\/?script>?/g.exec(str);
	return reg;
}
</script>
{/literal}
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

.thsub{
	font-size: 12px;
	margin-top: 5px;
}
</style>


{if $data.check.error}
<div class="alert alert-danger">
	{$data.check.error}
</div>
{/if}

<div class="text-right">
	{makelink mode="ad" action="listing" value="一覧へ戻る" class="btn btn-warning"}
</div>
<br>

<form name="f" action="" method="POST" onsubmit="return checkXSS();">
	<table class="table table-bordered">
		<tbody>
			<tr>
				<th  width="20%">ID</th>
				<td colspan="2">
					{$data.db.ad_id}
				</td>
			</tr>
			<tr>
				<th>広告名</th>
				<td colspan="2">
					<input type="text" name="ad_name" value="{$data.db.ad_name}" class="form-control">
					{if $data.check.field_errors.ad_name}
					<div class="alert alert-danger">{$data.check.field_errors.ad_name}</div>
					{/if}
				</td>
			</tr>
			<tr>
				<th>開始日時</th>
				<td>
					<input type="text" name="ad_date_begin" value="{$data.db.ad_date_begin}" class="form-control">
					{if $data.check.field_errors.ad_date_begin}
					<div class="alert alert-danger">{$data.check.field_errors.ad_date_begin}</div>
					{/if}
				</td>
				<td>
					<div class="alert alert-info">
						<b>例：</b>YYYY-MM-DD hh:mm:ss
					</div>
				</td>
			</tr>
			<tr>
				<th>終了日時</th>
				<td>
					<input type="text" name="ad_date_end" value="{$data.db.ad_date_end}" class="form-control">
					{if $data.check.field_errors.ad_date_end}
					<div class="alert alert-danger">{$data.check.field_errors.ad_date_end}</div>
					{/if}
				</td>
				<td>
					<div class="alert alert-info">
						<b>例：</b>YYYY-MM-DD hh:mm:ss
					</div>
				</td>
			</tr>
			<tr>
				<th>表示制御</th>
				<td colspan="2">
					<input type="hidden" name="ad_is_show" value="0">
					<label class="radio-label">
						<input type="checkbox" name="ad_is_show" value="1" {if $data.db.ad_is_show=='1'}checked{/if}> 表示する
					</label>
				</td>
			</tr>
			<tr>
				<th colspan="4" class="text-center">広告コード</th>
			</tr>
			<tr>
				<th colspan="4" class="alert alert-info"><p>Chrome使用時のXSSフィルターの対策で、「script」タグは<br>「 [script]　～ [/script] 」のように記述して登録してください。表示側で正しく置換されます。</p></th>
			<tr>
			<tr>
				<th>PC/タブレット&nbsp;<i class="fa fa-lg fa-desktop"></i><p class="thsub">PC,タブレットのブラウザ,タブレット使用時のアプリ内</p></th>
				<td colspan="2">
					<textarea name="ad_tag" rows="4" class="form-control">{$data.db.ad_tag}</textarea>
					{if $data.check.field_errors.ad_tag}
					<div class="alert alert-danger">{$data.check.field_errors.ad_tag}</div>
					{/if}
				</td>
			</tr>
			<tr>

			<tr>
				<th>モバイル&nbsp;<i class="fa fa-2x fa-mobile"></i><p class="thsub">スマホのブラウザから表示</p></th>
				<td colspan="2">
					<textarea name="ad_tag_mobile" rows="4" class="form-control">{$data.db.ad_tag_mobile}</textarea>
					{if $data.check.field_errors.ad_tag_mobile}
					<div class="alert alert-danger">{$data.check.field_errors.ad_tag_mobile}</div>
					{/if}
				</td>
			</tr>
			<tr>

			<tr>
				<th>Androidアプリ&nbsp;<i class="fa fa-lg fa-android"></i><p class="thsub">※タブレット時は「PC/タブレット」のタグ</p></th>
				<td colspan="2">
					<textarea name="ad_tag_Android" rows="4" class="form-control">{$data.db.ad_tag_Android}</textarea>
					{if $data.check.field_errors.ad_tag_Android}
					<div class="alert alert-danger">{$data.check.field_errors.ad_tag_Android}</div>
					{/if}
				</td>
			</tr>
			<tr>

			<tr>
				<th>iOSアプリ&nbsp;<i class="fa fa-lg fa-apple"></i><p class="thsub">※タブレット時は「PC/タブレット」のタグ</p></th>
				<td colspan="2">
					<textarea name="ad_tag_iOS" rows="4" class="form-control">{$data.db.ad_tag_iOS}</textarea>
					{if $data.check.field_errors.ad_tag_iOS}
					<div class="alert alert-danger">{$data.check.field_errors.ad_tag_iOS}</div>
					{/if}
				</td>
			</tr>
			<tr>

				<th>コメント</th>
				<td colspan="2">
					<textarea name="comment" rows="4" class="form-control">{$data.db.comment}</textarea>
				</td>
			</tr>
		</tbody>
	</table>

	<div class="text-center">
		<input type="hidden" name="mode" value="ad">
		<input type="hidden" name="action" value="check"> <!-- insert -->
		<input type="hidden" name="ad_id" value="{$data.db.ad_id}">
		<input type="submit" value="登録" class="btn btn-primary">
	</div>
</form>

{/block}
