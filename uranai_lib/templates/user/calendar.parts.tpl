<!--カレンダー機能-->
	<div class="calendar_box">
		<div class="rank-day-links">
			{insert previous_month_links mode='rank' topic = $data.data_type m=$previous_month star=$details_star}
			
				<span class="font-color">{$calendar.2}</span>
			{insert next_month_links mode='rank' topic = $data.data_type m=$next_month star=$details_star}
		</div>
		<div class="spadding">
			<div class="calendar">
				{foreach $calendar.1 as $week}
				<span class="font_bold">{$week}</span>
				{/foreach}
				{foreach $calendar.0 as $num}
				{$num}
				{/foreach}
			</div>
		</div>
		<div class="font-color font_bold calendar_description">
		確認したい日付を<br />クリックしてください♪{$data_type}
		</div>
	</div>
<!--カレンダー機能-->
