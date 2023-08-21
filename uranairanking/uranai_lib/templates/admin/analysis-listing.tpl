{extends file="main.tpl"}
{* block name=title}{/block *}
{block name=body}
{* TODO: LISTING *}

<script>
function setToday(){
	var today = toYmd(new Date());
	var s = document.getElementById("date_s");
	var e = document.getElementById("date_e");
	s.value = today; 
	e.value = today; 
	return false;
}

function setPreviousWeek(){
	var beforeOneWeek = new Date(new Date().getTime() - 60 * 60 * 24 * 7 * 1000)
		, day = beforeOneWeek.getDay()
		, diffToMonday = beforeOneWeek.getDate() - day + (day === 0 ? -6 : 1)
		, lastMonday = new Date(beforeOneWeek.setDate(diffToMonday))
		, lastSunday = new Date(beforeOneWeek.setDate(diffToMonday + 6));

	var monday = toYmd(lastMonday);
	var sunday = toYmd(lastSunday);

	var s = document.getElementById("date_s");
	var e = document.getElementById("date_e");
	s.value = monday; 
	e.value = sunday; 
	return false;

}

function toYmd(dt){
  var y = dt.getFullYear();
  var m = ("00" + (dt.getMonth()+1)).slice(-2);
  var d = ("00" + dt.getDate()).slice(-2);
  var result = y + "-" + m + "-" + d;
  return result;
}
</script>

<style>
th{
	background:none;
}

th,td{
	color:black;
	font-size:13px;
}
</style>

<div>
	<form class="button" action="index.php" method="POST">
		<div class='text-14'>
			<div><span class='text-bold'>期間1:</span></div>
			<div class='flex flex-space'>
				<div>開始日<input id="date_s" class="form-control" name="analysis_date1_start" type="date" size="10" value="{$date1_start}"></div>
				<div>終了日<input id="date_e" class="form-control" name="analysis_date1_end" type="date" size="10" value="{$date1_end}"></div>
			</div>
			<div><span class='text-bold'>期間2:</span></div>
			<div class='flex flex-space'>
				<div>開始日<input id="date_s" class="form-control" name="analysis_date2_start" type="date" size="10" value="{$date2_start}"></div>
				<div>終了日<input id="date_e" class="form-control" name="analysis_date2_end" type="date" size="10" value="{$date2_end}"></div>
			</div>
		</div>
		<div style="margin-top:10px">
			<input type="hidden" name="mode" value="analysis">
			<input type="hidden" name="action" value="listing">
			<button class="btn-fa btn btn-success" type="submit">適用</button>
		</div>
</div>
<hr>
</form>
<form class="button" action="index.php" method="POST">
	<input type="hidden" name="mode" value="analysis">
	<input type="hidden" name="action" value="export">
	<button class="btn-fa btn btn-success" type="submit" style="float:right">CSV出力</button>
</form>

<table style="width:80%" class="table table-striped">
	<tr>
		<th>項目</th>
		<th colspan=2>値</th>
		<th>差分</th>
	</tr>
{foreach $data as $dt}
	{foreach $dt as $k => $d}

		<tr>
			<th>{$analysis_data_jpn.$k}</th>
			<td>{$data[0][$k]}</td>
			<td>{$data[1][$k]}</td>

			{* 項目が数値だったら差分もだす *}
			{if is_numeric($data[0][$k])}
				{assign "value_diff" $data[1][$k] - $data[0][$k]}
				<td {if $value_diff < 0}style='color:red;'{/if}>{$value_diff}</td> {* 負の数は赤でだす *}
			{else}
				<td></td>
			{/if}

		</tr>
	{/foreach}
{/foreach}
</table>
{/block}
