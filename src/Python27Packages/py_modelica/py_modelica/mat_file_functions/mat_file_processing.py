__author__ = 'pmeijer'

import numpy as np
from py_modelica.mat_file_functions.mat_file_to_dict import MatFile2Dict

# Rescue if the limit-checking should get stuck in an infinite while-loop.
# Which should be impossible to start with, but if I am wrong...
MAX_ITERATIONS = 100000


class MatFileProcessing(object):
    """
    Class for processing the data read in from the mat-file
    (uses MatFile2Dict to created dictionary)
    """

    variable_filter = []  # list of all variables/parameter to load from mat-file
    # (does not need to include 'time' - loaded by default)
    time = None
    result = None

    def __init__(self, mat_file='', variable_filter=None):
        """
        Loads in mat-file, extracts given variables in variable_filter (time always included)
        and converts lists of values into numpy arrays.

        These are stored in result as:

        {{name1: array([values1])}, ..., {nameN: array([valuesN])}}

        """

        mat_converter = MatFile2Dict(mat_file, variable_filter, False)
        result_lists = mat_converter.get_results()

        # convert lists into numpy arrays
        self.result = {}
        for item in result_lists.iteritems():
            self.result.update({item[0]: np.array(item[1])})

        self.time = self.result['time']

    def data_array(self, name):
        """
        Get time-series in numpy array format.
        name - name of variable

        e.g. data_array('time')
        returns with the time.
        """

        return self.result[name]

    def time_array(self):
        """
        Get time-series of time in numpy array format.
        """

        return self.time

    def print_time(self):
        """
        Prints and returns with time-series of time.
        """

        time = self.time
        print 'here are time intervals:', time

        return time

    def short_array(self, name, start=0, end=-1):
        """
        Get a truncated, from n1 to n2 array for variable name

        name - name of variable
        start - start index of interval
        end - end index of interval

        N.B index goes from 0 to len(array)-1
        """

        return self.result[name][start:end]

    def plot(self, name):
        """
        Returns a tuple, suitable for plotting, of the variable's time-series together with time.

        name - name of variable
        """

        return self.data_array(name), self.time

    def get_data_by_time(self, name, time_val):
        """
        Get data based on time value.
        name - name of variable to consider
        time_val - time point where to extract the value

        Returns the data and the index of the data
        """
        i = 0
        time = self.time
        while time[i] < time_val and i in xrange(time.size - 1):
            i += 1
        data_arr = self.data_array(name)
        if time[i - 1] != time_val:
            cur = data_arr[i - 1]
            nxt = data_arr[i]
            data = time[i - 1] / ((time[i - 1] + time[i]) / 2) * (nxt - cur) + cur
        else:
            data = data_arr[i - 1]

        return data, i

    def get_data_by_index(self, name, index):

        return self.data_array(name)[index]

    def get_index_from_time(self, time_val):
        """
        Get index based on time value.
        time_val - time point where to extract the value

        Returns index nearest to time_val
        """
        i = 0
        time = self.time
        while time[i] < time_val and i in xrange(time.size-1):
            i += 1

        return i

    def get_time(self, name, value, atol=1e-4, rtol=1e-4, start_index=0, end_index=-1):
        """
        Gets the first time point where the variable satisfies either atol or rtol,
        if no such point exists - returns with -1.

        name - name of variable
        atol - absolute tolerance
        rtol - relative tolerance
        """

        index = -1
        # N.B. this is only one of many ways to do this
        denominator = 1
        if value > rtol:
            denominator = value

        data = self.data_array(name)[start_index:end_index]
        cnt = 0
        for x in data:
            abs_diff = abs(x - value)
            rel_diff = abs_diff / denominator
            if abs_diff < atol or rel_diff < rtol:
                index = cnt
                break
            else:
                cnt += 1

        if index >= 0:
            return self.time[start_index + index]

        return -1

    def last_value(self, name):
        """
        Get last value of variable
        name - name of variable
        """

        return self.data_array(name)[-1]

    def global_max(self, name):
        """
        Get maximum value of variable
        name - name of variable
        """

        return self.data_array(name).max()

    def global_max_time(self, name):
        """
        Get time where max occurs
        name - name of variable

        returns the time at where the max is
        """
        index = self.data_array(name).argmax()
        time_at_max = self.time[index]

        return time_at_max

    def global_min(self, name):
        """
        Get minimum value of variable
        name - name of variable
        """

        return self.data_array(name).min()

    def global_min_time(self, name):
        """
        Get time where min occurs
        name - name of variable

        returns the time at where the min is
        """
        index = self.data_array(name).argmin()
        time_at_min = self.time[index]

        return time_at_min

    def global_abs_max(self, name):
        """
        Get the maximum absolute value of variable
        name - name of variable
        """

        return np.absolute(self.data_array(name)).max()

    def std_dev(self, name):
        """
        Returns the standard deviation of variable
        name - name of variable
        """
        stddev = self.data_array(name).std()

        return stddev

    def variance(self, name):
        """
        Returns the variance of variable
        name - name of variable
        """
        variance = self.data_array(name).var()

        return variance

    def sum_value(self, name):
        """
        Returns the sum of the time-series for the variable
        name - name of variable
        """
        result = self.data_array(name).sum()

        return result

    def mean(self, name):
        """
        Returns the mean of the time-series for the variable
        name - name of variable
        """

        result = np.mean(self.data_array(name), dtype=np.float64)

        return result

    def integrate(self, name):
        """
        Returns the area under the curve of the time-series for the variable
        name - name of variable
        """

        time = self.time
        data = self.data_array(name)
        total_sum = 0
        nxt_t = time[0]
        for i in xrange(data.size):
            cur = next
            nxt = data[i]
            cur_t = nxt_t
            nxt_t = time[i]
            height = (nxt + cur) / 2
            interval = nxt_t - cur_t
            total_sum += height * interval

        return total_sum

    def minima(self, name):
        """
        Returns the minima of time-series of variable
        name - name of variable
        """

        data = self.data_array(name)
        minima = []
        prev = 0
        cur = 0
        nxt = data[0]
        for i in xrange(data.size):
            if cur < prev and cur <= nxt:
                minima.append(cur)
            prev = cur
            cur = nxt
            nxt = data[++i]
        minimum = np.array(minima)

        return minimum

    def maxima(self, name):
        """
        Returns the maxima of time-series of variable
        name - name of variable
        """
        data = self.data_array(name)
        maxima = []
        prev = 0
        cur = 0
        nxt = data[0]
        for i in xrange(data.size):
            if cur >= prev and cur > nxt:
                maxima.append(cur)
            prev = cur
            cur = nxt
            nxt = data[++i]

        maximum = np.array(maxima)

        return maximum

    def pos_neg(self, name, tol=0.00000015):
        """
        Returns time of the roots from positive to negative of time-series of variable
        name - name of variable
        tol - tolerance
        """

        data = self.data_array(name)
        time_arr = self.time
        time = []

        nxt = -1
        for i in xrange(data.size):
            cur = nxt
            nxt = data[i]
            if (cur > 0 + tol) and (nxt <= 0 + tol):
                if cur != 0:
                    cur_t = time_arr[i - 1]
                    next_t = time_arr[i]
                    time.append((cur / (cur + nxt) / 2) * (next_t - cur_t) + cur_t)
                else:
                    time.append(time_arr[i - 1])
        timing = np.array(time)

        return timing

    def neg_pos(self, name, tol=0.00000015):
        """
        Returns time of the roots from negative to positive of time-series of variable
        name - name of variable
        tol - tolerance
        """
        time = []
        data = self.data_array(name)
        time_arr = self.time
        nxt = 1
        for i in xrange(data.size):
            cur = nxt
            nxt = data[i]
            if (cur <= 0 + tol) and (nxt > 0 + tol):
                if cur != 0:
                    cur_t = time_arr[i - 1]
                    next_t = time_arr[i]
                    time.append(cur / ((cur + nxt) / 2) * (next_t - cur_t) + cur_t)
                else:
                    time.append(time_arr[i - 1])
        timing = np.array(time)

        return timing

    def to_zero(self, name, value_index):
        """
        # time from a number to zero
        # (use index from print_data() function)
        # parameters: data array, time array, index of value
        # returns the time of the zero
        """

        data = self.data_array(name)
        time_arr = self.time
        i = value_index + 1
        cur = data[value_index]
        nxt = data[i]
        tolerance = 0.00000015
        if data[value_index] >= 0:
            while nxt >= 0 + tolerance and i in xrange(data.size - 1):
                i += 1
                cur = nxt
                nxt = data[i]
            if nxt >= 0 + tolerance:
                return -1
        else:
            while nxt <= 0 + tolerance and i in xrange(data.size - 1):
                i += 1
                cur = nxt
                nxt = data[i]
            if nxt <= 0 + tolerance:
                return -1
        if cur != 0:
            cur_t = time_arr[i - 1]
            next_t = time_arr[i]
            time = cur / ((cur + nxt) / 2) * (next_t - cur_t) + cur_t
        else:
            time = time_arr[i - 1]

        return time

    def from_zero(self, name, value_index):
        """
        # time from a number to zero
        # (use index from print_data() function)
        # parameters: data array, time array, index of value
        # returns the time of the zero
        """

        data = self.data_array(name)
        time_arr = self.time

        i = value_index - 1
        cur = data[value_index]
        nxt = data[i]
        tolerance = 0.00000015
        if data[value_index - 1] >= 0:
            while nxt >= 0 + tolerance and i in xrange(data.size):
                i -= 1
                cur = nxt
                nxt = data[i]
            if nxt >= 0 + tolerance:
                return -1
        else:
            while nxt <= 0 + tolerance and i in xrange(data.size):
                i -= 1
                cur = nxt
                nxt = data[i]
            if nxt <= 0 + tolerance:
                return -1
        if cur != 0:
            cur_t = time_arr[i + 1]
            next_t = time_arr[i]
            time = cur / ((cur + nxt) / 2) * (next_t - cur_t) + cur_t
        else:
            time = time_arr[i + 1]

        return time

    def zeros(self, name):
        """
        Find zeros of time-series for variable
        name - name of variable

        returns the time of the zero
        """
        data_array = self.data_array(name)
        time = self.time
        data = [[], []]
        data[0].append(self.pos_neg(data_array, time))
        data[1].append(self.neg_pos(data_array, time))
        data_arr = np.array(data)

        return data_arr

    def compare(self, name1, name2):
        """
        Compare the time-series of two variables
        name1 - name of variable 1
        name2 - name of variable 2

        returns true if the results are identical
        """
        data1 = self.data_array(name1)
        data2 = self.data_array(name2)
        for i in xrange(data1.size):
            if data1[i] != data2[i]:
                return False

        return True

    def time_total(self, val1, val2):
        # finding the difference between 2 times
        time = abs(val2 - val1)

        return time

    def delta_t(self, start_index, end_index):
        """
        Returns the length of the time-interval between to indices
        """
        t1 = self.time[start_index]
        t2 = self.time[end_index]
        dt = t2 - t1

        return dt

    def get_local_max(self, name, start_index, end_index):
        """
        Returns the value of the maximum between two indices
        N.B. including both points
        :param name:
        :param start_index:
        :param end_index:
        """

        if end_index == -1:
            maximum = self.data_array(name)[start_index:].max()
        else:
            maximum = self.data_array(name)[start_index:end_index + 1].max()

        return maximum

    def get_local_min(self, name, start_index, end_index):
        """
        Returns the value of the minimum between two indices
        N.B. including both points
        """

        if end_index == -1:
            minimum = self.data_array(name)[start_index:].min()
        else:
            minimum = self.data_array(name)[start_index:end_index + 1].min()

        return minimum

    def find_first_max_violation(self, name, value, start_index=0):
        """
        Starting from start_index it looks for the first index where the
        time-series has a value greater than value.

        If it never occurs, it returns -1
        """

        time_series = self.data_array(name)[start_index:]

        n = len(time_series)
        for i in range(n):
            if time_series[i] > value:
                return i + start_index

        return -1

    def find_first_min_violation(self, name, value, start_index=0):
        """
        Starting from start_index it looks for the first index where the
        time-series has a value less than value.

        If it never occurs, it returns -1
        """
        time_series = self.data_array(name)[start_index:]

        n = len(time_series)
        for i in range(n):
            if time_series[i] < value:
                return i + start_index

        return -1

    def check_max_limit(self, name, value, duration=0.5):
        """
        Checks if the variable 'name' is greater than 'value' for a time period of
        at least the 'duration' in seconds.

        :param name:
        :param value:
        :param duration:
        """
        actual_value = ''
        limit_exceeded = False
        start_index = 0
        global_max = -np.Inf
        cnt = 0
        print 'check_max_limit'
        while start_index > -1:
            index = self.find_first_max_violation(name, value, start_index)
            if index > -1:
                end_index = self.find_first_min_violation(name, value, index)
                d_t = self.delta_t(index, end_index)
                print 'Found violation at t={0} lasting : {1}'.format(self.time[index], d_t)
                if d_t >= duration:
                    limit_exceeded = True
                    local_max = self.get_local_max(name, index, end_index)
                    print 'Local maximum : {0}'.format(local_max)
                    if local_max > global_max:
                        global_max = local_max
                start_index = end_index
            else:
                break

            cnt += 1
            if cnt == MAX_ITERATIONS:
                msg = 'Limit checking for variable {0} aborted after {1} iterations'.format(name, MAX_ITERATIONS)
                print msg
                raise StopIteration(msg)

        if limit_exceeded:
            actual_value = global_max

        return limit_exceeded, actual_value

    def check_min_limit(self, name, value, duration=0.5):
        """
        Checks if the variable 'name' is smaller than 'value' for a time period of
        at least the 'duration' in seconds.

        :param name:
        :param value:
        :param duration:
        """
        actual_value = ''
        limit_exceeded = False
        start_index = 0
        global_min = np.Inf
        cnt = 0
        print 'check_min_limit'
        while start_index > -1:
            index = self.find_first_min_violation(name, value, start_index)
            if index > -1:
                end_index = self.find_first_max_violation(name, value, index)
                d_t = self.delta_t(index, end_index)
                print 'Found violation at t={0} lasting : {1} s'.format(self.time[index], d_t)
                if d_t >= duration:
                    limit_exceeded = True
                    local_min = self.get_local_min(name, index, end_index)
                    print 'Local minimum : {0}'.format(local_min)
                    if local_min < global_min:
                        global_min = local_min
                start_index = end_index
            else:
                break

            cnt += 1
            if cnt == MAX_ITERATIONS:
                msg = 'Limit checking for variable {0} aborted after {1} iterations'.format(name, MAX_ITERATIONS)
                print msg
                raise StopIteration(msg)

        if limit_exceeded:
            actual_value = global_min

        return limit_exceeded, actual_value
