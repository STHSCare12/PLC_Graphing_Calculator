#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"
#include "evaluator.h"
#include "helper_functions/pbPlots/pbPlots.h"
#include "helper_functions/pbPlots/supportLib.h"
#include <math.h>
#include <float.h>

// Helper function to detect if a node contains trig functions
int contains_trig_functions(ASTNode *node)
{
    if (!node)
        return 0;

    if (node->type == 'f' &&
        (strcmp(node->func, "sin") == 0 || strcmp(node->func, "cos") == 0 || strcmp(node->func, "tan") == 0))
    {
        return 1;
    }

    return contains_trig_functions(node->left) || contains_trig_functions(node->right);
}

// Helper to find appropriate x-range for trig functions
void find_x_range(ASTNode *node, double *x_min, double *x_max, int *points)
{
    if (contains_trig_functions(node))
    {
        // For trig functions, show multiple periods
        *x_min = -2 * 180;
        *x_max = 2 * 180;
        *points = 2000;
    }
    else
    {
        // Default range for non-trig functions
        *x_min = -10.0;
        *x_max = 10.0;
        *points = 1000;
    }
}

// Helper to check if a point is valid (not near asymptote)
int is_valid_point(double y, double prev_y)
{
    if (!isfinite(y))
        return 0;

    // Detect asymptotes by checking for large jumps in y values
    if (fabs(y - prev_y) > 1000 && fabs(y) > 100)
    {
        return 0;
    }

    return 1;
}

void plot_graph(ASTNode *node)
{
    double x_min, x_max;
    int length;
    char filename[100];

    // Auto-detect range based on function type
    find_x_range(node, &x_min, &x_max, &length);

    double *x = malloc(sizeof(double) * length);
    double *y = malloc(sizeof(double) * length);
    double step = (x_max - x_min) / (length - 1);

    // Find y range
    double y_min = INFINITY, y_max = -INFINITY;

    // For detecting asymptotes
    double prev_y = 0;
    int valid_count = 0;

    // Evaluate x and y values
    for (int i = 0; i < length; i++)
    {
        x[i] = x_min + i * step;
        CURRENT_X = x[i];
        y[i] = evaluate_expression(node);

        // Check if point is valid
        if (is_valid_point(y[i], prev_y))
        {
            if (y[i] < y_min)
                y_min = y[i];
            if (y[i] > y_max)
                y_max = y[i];
            valid_count++;
        }

        prev_y = y[i];
    }

    if (valid_count == 0)
    {
        printf("Error: No valid points to graph!\n");
        free(x);
        free(y);
        return;
    }

    // Handling for trig functions
    if (contains_trig_functions(node))
    {
        // For sin and cos functions
        if (contains_trig_functions(node) && !contains_trig_functions(node->left) && !contains_trig_functions(node->right))
        {
            y_min = -1.2;
            y_max = 1.2;
        }
        // For tan and cot functions
        else if (contains_trig_functions(node) && (strstr(node->func, "tan") || strstr(node->func, "cot")))
        {
            // Clip extreme values for tan
            double max_abs = fmax(fabs(y_min), fabs(y_max));
            if (max_abs > 10)
            {
                y_min = -10;
                y_max = 10;
            }
        }
        else
        {
            // General case with padding
            double padding = 0.2;
            double y_range = y_max - y_min;
            y_min = y_min - (y_range * padding);
            y_max = y_max + (y_range * padding);
        }
    }
    else
    {
        // padding for non-trig functions
        double padding = 0.1;
        double y_range = y_max - y_min;
        if (y_range > 0)
        {
            y_min = y_min - (y_range * padding);
            y_max = y_max + (y_range * padding);
        }
        else
        {
            y_min = y_min - 1;
            y_max = y_max + 1;
        }
    }

    // Ensure axes are visible if function crosses zero
    if (y_min > 0)
        y_min = 0;
    if (y_max < 0)
        y_max = 0;

    // Create series
    ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
    series->xs = x;
    series->xsLength = length;
    series->ys = y;
    series->ysLength = length;
    series->linearInterpolation = true;
    series->color = CreateRGBColor(0.2, 0.3, 0.8);

    ScatterPlotSeries *seriesArray[1];
    seriesArray[0] = series;

    // Plot settings
    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
    settings->width = 800;
    settings->height = 600;
    settings->xMin = x_min;
    settings->xMax = x_max;
    settings->yMin = y_min;
    settings->yMax = y_max;
    settings->autoBoundaries = false;
    settings->scatterPlotSeries = seriesArray;
    settings->scatterPlotSeriesLength = 1;

    // Draw graph and save
    RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();
    DrawScatterPlotFromSettings(imageRef, settings, NULL);

    ByteArray *pngdata = ConvertToPNG(imageRef->image);

    printf("\nGraph generated successfully!\n");

    // Ask user for filename
    printf("Enter filename to save graph (default name: graph.png): ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    // Use default if no filename provided
    if (strlen(filename) == 0)
    {
        strcpy(filename, "graph.png");
    }

    // Append .png if not provided
    if (strstr(filename, ".png") == NULL)
    {
        strcat(filename, ".png");
    }

    WriteToFile(pngdata, filename);

    // Cleanup
    DeleteImage(imageRef->image);
    free(x);
    free(y);

    printf("Graph successfully saved as %s\n", filename);
}
