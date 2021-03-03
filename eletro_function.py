def unpack_data(inc_data):
    dep_data = inc_data.split()
    dep_data[1] = float(dep_data[1])
    dec_data = {dep_data[0]: dep_data[1]}
    return dec_data


def avg_list(list):
    avg_list = (sum(list))/len(list)
    avg_list_out = float(format(avg_list, '.2f'))
    return avg_list_out


def decode_format(data):
    decode_data = data.decode("utf-8")
    out_data = decode_data[0:][:-2]
    return out_data


